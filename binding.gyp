{
    "targets": [{
        "target_name": "binding",
        "sources": [
            "src/binding.cc"
        ],
        "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "C:/development/products/cspice/include"],
        "libraries": [
            "C:/development/products/cspice/lib/cspice.lib"
        ]
    }]
}
