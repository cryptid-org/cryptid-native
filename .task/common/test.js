const path = require('path');

const { compileAllSources, compileExecutableForComponent } = require('./compile');
const { removeFiles, run } = require('./util');


function runTests(dependencies, components) {
    try {
        const { output, errors } = testComponents(dependencies, components);

        if (errors.length > 0) {
            console.log(errors);
            throw new Error('There were errors running the tests. Please see the log lines above.');
        }

        emitTestResults(dependencies, output);
    } finally {
        removeFiles(dependencies, ['*.o', '*.out']);
    }
};

function testComponents(dependencies, components) {
    const errors = [];
    const output = [];

    compileAllSources(dependencies, ['-g']);

    for (const component of components) {
        console.log(`Testing ${component}`);

        const executable = compileExecutableForComponent(component, dependencies, []);

        try {
            const { stdout } = run(dependencies, executable, ['-v'], {
                stdio: ['inherit', 'pipe', 'inherit']
            });

            const stdoutString = stdout.toString();

            console.log(stdoutString);
            output.push({
                component,
                stdout: stdoutString
            });
        } catch (e) {
            errors.push({
                component,
                error: e
            });

            console.log('DEBUG - ERR');
            console.log(e.stdout);
        } finally {
            removeFiles(dependencies, [executable]);
        }

        console.log('\n');
    }

    return {
        output,
        errors
    };
};

function emitTestResults(dependencies, output) {
    const { fs, paths } = dependencies;

    fs.ensureDirSync(paths.test.resultsDirectory);

    output.forEach(singleOutput => writeOutputToXml(dependencies, singleOutput));
};

function writeOutputToXml(dependencies, { component, stdout }) {
    const tapOutput = tapifyGreatest(dependencies, stdout);

    const xmlOutput = xmlifyTap(dependencies, component, tapOutput);

    const { fs, paths } = dependencies;

    fs.writeFileSync(path.join(paths.test.resultsDirectory, `${component}.xml`), xmlOutput);   
};

function tapifyGreatest({ paths, spawnSync }, input) {
    const { stdout } = run({ spawnSync }, paths.test.tapify, [], {
        input,
        stdio: [],
        cwd: paths.test.resultsDirectory 
    });

    return stdout;
};

function xmlifyTap({ tapToJUnit }, component, input) {
    return tapToJUnit(input, component);
};

module.exports = {
    runTests
};
