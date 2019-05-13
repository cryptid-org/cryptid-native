const path = require('path');

const { compileAllSources, compileExecutableForComponent } = require('./compile');
const { removeFiles, run } = require('./util');


const TEN_MEGABYTES = 10 * 1024 * 1024;

function runTests(dependencies, components) {
    try {
        const { output, errors } = testComponents(dependencies, components);

        if (errors.length > 0) {
            console.log(errors);
            throw new Error('There were errors running the tests. Please see the log lines above.');
        }

        console.log(errors);

        console.log('emitting');

        try {
            emitTestResults(dependencies, output);
        } catch (e) {
            console.log('ERR');
            console.log(e);
        }
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
                stdio: ['inherit', 'pipe', 'inherit'],
                maxBuffer: TEN_MEGABYTES
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

            console.log('ERROR testing component:');
            console.log(e.error);
            console.log(e.error.message);
            console.log(e.stdout.toString());
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

    console.log(tapOutput);

    const xmlOutput = xmlifyTap(dependencies, component, tapOutput);

    console.log(xmlOutput);

    const { fs, paths } = dependencies;

    fs.writeFileSync(path.join(paths.test.resultsDirectory, `${component}.xml`), xmlOutput);   
};

function tapifyGreatest({ paths, spawnSync }, input) {
    const result = run({ spawnSync }, 'gawk', ['-f', paths.test.tapify], {
        input,
        stdio: [],
        cwd: paths.test.resultsDirectory 
    });

    console.log(result.error);
    console.log(result.stderr.toString());

    return result.stdout.toString();
};

function xmlifyTap({ tapToJUnit }, component, input) {
    return tapToJUnit(input, component);
};

module.exports = {
    runTests
};
