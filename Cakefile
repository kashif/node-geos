{print} = require "util"
_spawn = (require "child_process").spawn

coffeeBin = "./node_modules/coffee-script/bin/coffee"
vows = "./node_modules/vows/bin/vows"
nodeGyp = "./node_modules/node-gyp/bin/node-gyp.js"

spawn = (command, options) ->
  program = _spawn command, options
  program.stdout.on "data", (data) -> print data.toString()
  program.stderr.on "data", (data) -> print data.toString()
  program #return program

buildBinary = ->
  spawn nodeGyp, ["rebuild"]

build = (src, dst, watch) ->
  options = ["-c", "-o", dst, src]
  options.unshift "-w" if watch

  spawn coffeeBin, options

task "binary", "Compile C++ Code", ->
  buildBinary()

task "build", "Compile CoffeeScript source files", ->
  build "src", "lib"
  build "examples", "examples"

task "install", "Compile CoffeeScript and C++ Code", ->
  buildBinary()
  build "src", "lib"
  build "examples", "examples"

task "watch", "Recompile CoffeeScript source files when modified", ->
  build "src", "lib", true
  build "examples", "examples", true

task "test", "Run the geos tests", ->
  spawn vows, ["--spec"]

task "clean", "Remove all \"binary\" data", ->
  spawn nodeGyp, ["clean"]
  spawn "rm", ["./lib/index.js"]
