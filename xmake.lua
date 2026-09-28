set_project("uwu")

set_languages("c99")
add_cflags("-Wall", "-Wextra", "-pedantic", "-g",
           "-fno-omit-frame-pointer", "-D_ISOC99_SOURCE")

target("libuwu")
	set_kind("static")
	add_files("src/*.c")

target("uwuify")
	set_kind("binary")
	add_files("cmd/uwuify.c")
	add_deps("libuwu")
	add_includedirs(".")

target("tests")
	set_kind("binary")
	add_files("tests/main.c")
	add_deps("libuwu")
	add_includedirs(".")
	add_cflags("-fsanitize=address,undefined",
		   "-fno-omit-frame-pointer")
	add_ldflags("-fsanitize=address,undefined")
