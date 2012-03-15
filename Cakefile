{print} = require "util"
{spawn, exec} = require "child_process"

coffeeBin = "./node_modules/coffee-script/bin/coffee"
vows = "./node_modules/vows/bin/vows"

buildBinary = ->
  waf = spawn "node-waf", ["clean", "configure", "build"]
  waf.stdout.on "data", (data) -> print data.toString()
  waf.stderr.on "data", (data) -> print data.toString()

build = (src, dst, watch) ->
  options = ["-c", "-o", dst, src]
  options.unshift "-w" if watch

  coffee = spawn coffeeBin, options
  coffee.stdout.on "data", (data) -> print data.toString()
  coffee.stderr.on "data", (data) -> print data.toString()

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
  vows = spawn vows
  vows.stdout.on "data", (data) -> print data.toString()
  vows.stderr.on "data", (data) -> print data.toString()
