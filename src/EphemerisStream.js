'use strict';

var util = require('util');
var Readable = require('stream').Readable;

var cspice = require('../build/Release/binding');

var EphemerisStream = function(options) {
    Readable.call(this, { objectMode: true });

    if (Array.isArray(options.kernel)) {
        options.kernel.forEach(cspice.furnsh);
    }
    else {
        cspice.furnsh(options.kernel);
    }

    this._observer = options.observer;
    this._target = options.target;
    this._frame = options.frame || 'J2000';
    this._start = Date.parse(options.start);
    this._stop = Date.parse(options.stop);
    this._count = options.count;

    var d = this._stop - this._start;
    if (d < 0) {
        throw new Error('start must be before stop');
    }
    this._step = 0;
    if (this._count > 1) {
        this._step = d / (this._count - 1);
    }
    this._i = 0;
};

util.inherits(EphemerisStream, Readable);

EphemerisStream.prototype._read = function() {
    if (this._i < this._count) {
        var t = this._i++ * this._step;
        var epoch = new Date(this._start + t).toJSON().replace(/Z$/, '');
        try {
            var data = cspice.spkezr(this._observer, this._target, epoch, this._frame);
            for (var i=0; i<data.length; i++) {
                data[i] *= 1000.0;
            }
            data.unshift(t / 1000.0);
            this.push(data);
        }
        catch (ex) {
            this.emit('error', ex);
            return;
        }
    }
    else {
        this.push(null);
    }
};

module.exports = EphemerisStream;

