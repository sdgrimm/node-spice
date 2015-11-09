'use strict';

var Transform = require('stream').Transform;
var getEphemeris = require('../src/getEphemeris');

var data = {
    kernel: 'test/meta-kernel.txt',
    observer: 'moon',
    target: 'earth',
    frame: 'MOON_PA',
    start: '2015-11-01T00:00:00Z',
    stop: '2015-11-01T01:00:00Z',
    count: 10
};

var out = new Transform({ objectMode: true });
out._transform = function(obj, enc, cb) {
    console.log(JSON.stringify(obj, null, 0));
    cb(null);
};

getEphemeris(data).pipe(out);
