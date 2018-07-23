const addon = require('bindings')('myAddon');
const assert = require('assert');

try {
  addon.concat('hello')
}
catch(err) {
  assert(err.code === 'ERR_INVALID_ARG_TYPE');
  assert(err.message === 'Wrong number of arguments');
}

assert(addon.concat('Hello ', 'world!') === 'Hello world!');
addon.sumPromise()
  .then(value => {
    assert(value === 42);
  });