const { reportCoverage } = require('../common/coverage');


module.exports = {
    command: 'report-coverage',
    desc: 'Reports coverage data to Coveralls. Make sure, you run the coverage command prior to this one.',
    handlerFactory(dependencies) {
        return function handler() {
            reportCoverage(dependencies);
        };
    }
};
