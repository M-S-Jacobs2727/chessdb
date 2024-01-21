workspace "JChess"
    configurations {"Debug", "Release"}

project "JChess"
    kind "StaticLib"
    language "C++"
    targetdir "build/bin/%{cfg.buildcfg}"
    objdir "build/obj"

    links {"pq", "pqxx"}

    files {"src/**.h", "src/**.cpp"}
    removefiles {"src/engine/*", "src/internal/game/*", "src/fileFormat/*", "src/database/*"}

    pqxxdir = os.findheader("pqxx/pqxx")
    includedirs({"src", pqxxdir})

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
