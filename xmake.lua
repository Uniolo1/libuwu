-- SPDX-FileCopyrightText: NONE
-- SPDX-License-Identifier: Unlicense

set_project("uwu")

set_languages("c99")
set_warnings("all", "extra")

target("libuwu")
	set_kind("static")
	add_files("src/*.c")

target("uwuify")
	set_kind("binary")
	add_files("cmd/uwuify.c")
	add_deps("libuwu")
	add_includedirs(".")

-- NOTE: tests may fail to build with MSVC
target("tests")
	set_kind("binary")
	add_files("tests/main.c")
	add_deps("libuwu")
	add_includedirs(".")

	add_rules("mode.debug")
	add_cflags("-fsanitize=address,undefined",
	           "-fno-omit-frame-pointer")
	add_ldflags("-fsanitize=address,undefined")

task("test")
	on_run(function()
		os.exec("xmake run tests")
	end)
	set_menu({
		usage = "xmake test",
		description = "Run tests"
	})
task_end()
