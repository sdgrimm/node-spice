{
    "targets": [{
        "target_name": "binding",
        "sources": [
            "src/binding.cc"
        ],
        "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "$(CSPICE)/include"
        ],
        "conditions": [
            ['OS=="win"', {
                "libraries": [
                    "$(CSPICE)/lib/cspice.lib"
                 ]
            }, {
                "libraries": [
                    "$(CSPICE)/lib/cspice.a"
                 ]
            }]
        ]
    }]
}
