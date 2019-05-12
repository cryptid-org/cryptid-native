const { generateCoverage } = require('../common/coverage');
const { discoverAllComponents } = require('../common/util');


module.exports = {
    command: 'coverage [components...]',
    desc: 'Runs the tests of the specified components and generates LCOV and human-readable HTML coverage information. If no components are set, all of them will be tested.',
    handlerFactory(dependencies) {
        return function handler({ components = discoverAllComponents(dependencies) }) {
            generateCoverage(dependencies, components);
        };
    }
};
