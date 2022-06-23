target("ray_tracing")
    set_kind("binary")
    add_files("sources/*.cpp")
    add_includedirs("headers", "thirdparty")
