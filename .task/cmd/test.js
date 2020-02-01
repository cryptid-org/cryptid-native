const { runTestsWithCoverage } = require('../common/coverage');
const { runTests } = require('../common/test');
const { discoverAllComponents } = require('../common/util');


module.exports = {
    command: 'test [components...]',
    desc: 'Runs the tests of the specified components (separated by spaces). If no components are set, all of them will be tested.',
    builder: {
        coverage: {
            type: 'boolean',
            default: false,
            desc: 'Generate LCOV and human-readable HTML coverage information when running the tests.'
        }
    },
    handlerFactory(dependencies) {
        return function handler({ components = discoverAllComponents(dependencies), coverage }) {
            if (coverage) {
                runTestsWithCoverage(dependencies, components)
            } else {
                runTests(dependencies, components);
            }
        };
    }
};
