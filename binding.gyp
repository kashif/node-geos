{
  "targets": [
    {
      "target_name": "geos",
      "sources": [
        "src/cpp/binding.cpp",
        "src/cpp/geometry.cpp",
        "src/cpp/wktreader.cpp",
        "src/cpp/wktwriter.cpp",
        "src/cpp/geometryfactory.cpp",
        "src/cpp/precisionmodel.cpp",
        "src/cpp/quadtree.cpp",
        "src/cpp/geojsonwriter.cpp",
        "src/cpp/geojsonreader.cpp"
      ],
      'conditions': [
        ['OS=="win"', {
          # no Windows support yet...
        }, {
          'libraries': [
            '<!@(geos-config --libs)'
          ],
        }],
        ['OS=="mac"', {
          # cflags on OS X defined like this TODO: check
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '<!@(geos-config --cflags)'
            ]
          }
        }, {
          'cflags': [
            '<!@(geos-config --cflags)'
          ],
        }]
      ]
    }
  ]
}