workspace "JChess"
    architecture "x64"
    language "C++"

    configurations {"Debug", "Release"}
    
    includedirs "src"
    location "build"

    locdir = "build/%{prj.name}/" --%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
    
    filter "action:gmake2"
        buildoptions {"-std=c++23", "-Wall", "-Waddress"}
    
    filter {"action:gmake2", "configurations:Debug"}
        buildoptions {"-g", "-Og"}
    
    filter {"action:gmake2", "configurations:Release"}
        buildoptions {"-O3"}

    filter {}
    
project "jchess-core"
    kind "StaticLib"

    location(locdir)
    targetdir "%{prj.location}"
    objdir "%{prj.location}/obj"

    files {"src/internal/**.cpp", "src/internal/**.h"}
    
project "jchess-engine"
    kind "StaticLib"

    location(locdir)
    targetdir "%{prj.location}"
    objdir "%{prj.location}/obj"

    links {"jchess-core"}

    files {"src/engine/**.cpp", "src/engine/**.h"}
    
project "jchess-database"
    kind "StaticLib"

    location(locdir)
    targetdir "%{prj.location}"
    objdir "%{prj.location}/obj"

    pqxxdir = os.findheader("pqxx/pqxx")
    includedirs({"src", pqxxdir})

    links {"jchess-core", "pq", "pqxx"}

    files {"src/database/**.cpp", "src/database/**.h"}

-- project "jchess-lib"
--     kind "StaticLib"

--     location(locdir)
--     targetdir "%{prj.location}"
--     objdir "%{prj.location}/obj"

--     links {"jchess-core", "jchess-engine", "jchess-database"}

--     files {"src/interface/**.cpp", "src/interface/**.h"}

project "jchess-test"
    kind "ConsoleApp"

    location(locdir)
    targetdir "%{prj.location}"
    objdir "%{prj.location}/obj"

    gtestdir = os.findheader("gtest/gtest.h")
    includedirs({"src", gtestdir})

    links {"jchess-core", "jchess-engine", "jchess-database", "gtest_main"}

    files "test/**.cpp"