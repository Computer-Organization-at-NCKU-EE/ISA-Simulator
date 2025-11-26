BUILD_DIR := "builddir"
BUILD_DIR_EXITST := if path_exists(BUILD_DIR) == "true" { "true" } else { "false" }

default: compile

# Meson setup
setup:
    @{{BUILD_DIR_EXITST}} || meson setup --reconfigure {{BUILD_DIR}}
# Meson compile with ninja
compile: setup
    @meson compile -C {{BUILD_DIR}}
clean:
    @rm -rf {{BUILD_DIR}}
