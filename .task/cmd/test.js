const { runTests } = require('../common/test');
const { discoverAllComponents } = require('../common/util');


module.exports = {
    command: 'test [components...]',
    desc: 'Runs the tests of the specified components (separated by spaces). If no components are set, all of them will be tested.',
    handlerFactory(dependencies) {
        return function handler({ components = discoverAllComponents(dependencies) }) {
            runTests(dependencies, components);
        };
    }
};
