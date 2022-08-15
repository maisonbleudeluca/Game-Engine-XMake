add_rules("mode.debug", "mode.release")
set_languages("cxx17")

if is_mode("debug") then
    add_defines("DEBUG")
    set_optimize("none")
elseif is_mode("release") then
    add_defines("NDEBUG")
    set_optimize(fastest)
end

local packages = {"glfw", "glm", "assimp", "stb", "spdlog"}
add_requires(packages)
add_rules("plugin.vsxmake.autoupdate")

target("Engine")
    set_kind("binary")
    set_rundir("$(projectdir)")
    add_files("src/*.cpp")
    add_packages(packages)
target_end()