const { generateCoverage, reportCoverage } = require('../common/coverage');
const { discoverAllComponents } = require('../common/util');


module.exports = {
    command: 'coverage [components...]',
    builder: {
        report: {
            desc: 'Report coverage to Coveralls.'
        }
    },
    desc: 'Runs the tests of the specified components and generates LCOV and human-readable HTML coverage information. If no components are set, all of them will be tested.',
    handlerFactory(dependencies) {
        return function handler({ components = discoverAllComponents(dependencies), report }) {
            generateCoverage(dependencies, components);

            if (report) {
                reportCoverage(dependencies);
            }
        };
    }
};
