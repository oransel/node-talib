/**
 * Basic tests for node-talib
 * Run with: node --test test/basic.test.js
 */

const { test, describe } = require('node:test');
const assert = require('node:assert');
const talib = require('../index.js');

describe('TALib Module', () => {
  test('should export version', () => {
    assert.ok(talib.version);
    assert.strictEqual(typeof talib.version, 'string');
  });

  test('should export functions array', () => {
    assert.ok(Array.isArray(talib.functions));
    assert.ok(talib.functions.length > 0);
  });

  test('should export functionUnstIds', () => {
    assert.ok(talib.functionUnstIds);
    assert.strictEqual(typeof talib.functionUnstIds, 'object');
  });

  test('should have explain function', () => {
    assert.strictEqual(typeof talib.explain, 'function');
  });

  test('should have execute function', () => {
    assert.strictEqual(typeof talib.execute, 'function');
  });

  test('should have setUnstablePeriod function', () => {
    assert.strictEqual(typeof talib.setUnstablePeriod, 'function');
  });
});

describe('TALib Functions', () => {
  test('should explain SMA function', () => {
    const smaInfo = talib.explain('SMA');
    assert.strictEqual(smaInfo.name, 'SMA');
    assert.ok(smaInfo.group);
    assert.ok(smaInfo.hint);
    assert.ok(Array.isArray(smaInfo.inputs));
    assert.ok(Array.isArray(smaInfo.optInputs));
    assert.ok(Array.isArray(smaInfo.outputs));
  });

  test('should calculate SMA synchronously', () => {
    const data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    const result = talib.execute({
      name: 'SMA',
      startIdx: 0,
      endIdx: data.length - 1,
      inReal: data,
      optInTimePeriod: 3
    });

    assert.ok(result);
    assert.ok(result.result);
    assert.ok(result.result.outReal);
    assert.ok(Array.isArray(result.result.outReal));
    assert.ok(result.result.outReal.length > 0);
  });

  test('should calculate SMA asynchronously', (t, done) => {
    const data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    talib.execute(
      {
        name: 'SMA',
        startIdx: 0,
        endIdx: data.length - 1,
        inReal: data,
        optInTimePeriod: 3
      },
      (err, result) => {
        assert.ifError(err);
        assert.ok(result);
        assert.ok(result.result);
        assert.ok(result.result.outReal);
        assert.ok(Array.isArray(result.result.outReal));
        done();
      }
    );
  });

  test('should calculate RSI', () => {
    const data = [44, 44.34, 44.09, 43.61, 44.33, 44.83, 45.10, 45.42, 45.84, 46.08, 45.89, 46.03, 45.61, 46.28, 46.28];
    const result = talib.execute({
      name: 'RSI',
      startIdx: 0,
      endIdx: data.length - 1,
      inReal: data,
      optInTimePeriod: 14
    });

    assert.ok(result);
    assert.ok(result.result.outReal);
    assert.ok(result.result.outReal.length > 0);
  });
});
