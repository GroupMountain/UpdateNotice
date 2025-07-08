add_rules("mode.debug", "mode.release")

add_repositories(
    "levimc-repo https://github.com/LiteLDev/xmake-repo.git",
    "groupmountain-repo https://github.com/GroupMountain/xmake-repo.git"
)

add_requires("levilamina 1.3.3", { configs = {target_type = "server"} })

add_requires(
    "levibuildscript 0.4.1",
    "gmlib 1.3.0"
)

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

option("target_type")
    set_default("server")
    set_showmenu(true)
    set_values("server", "client")
option_end()

target("UpdateNotice")
    add_rules("@levibuildscript/linkrule")
    add_rules("@levibuildscript/modpacker")
    add_packages(
        "levilamina",
        "gmlib"
    )
    add_cxflags(
        "/EHa",
        "/utf-8",
        "/W4"
    )
    add_defines(
        "NOMINMAX",
        "UNICODE"
    )
    add_includedirs("src")
    add_files(
        "src/**.cpp",
        "src/**.rc"
    )
    set_exceptions("none")
    set_kind("shared")
    set_languages("c++20")
    set_symbols("debug")