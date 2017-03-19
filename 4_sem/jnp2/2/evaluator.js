process.stdin.setEncoding('utf8');

let mappings = {};
let tree = { calc: () => { return 0; }};

function isOperation(ch) {
    let operations = ['+', '-', '*', '/'];
    return operations.indexOf(ch) > -1;
}

function getPriority(ch) {
    if (ch === '+')
        return 1;
    else if (ch === '-')
        return 1;
    else 
        return 2;
}

function getMapping(str) {
    return mappings[str];
}

function evaluate(str) {
    if (str.length == 0)
        throw 'Invalid expression'
    if (str[0] >= '0' && str[0] <= '9') {
        return parseNumber(str);
    } else {
        return getMapping(str);
    }
}

function parseNumber(str) {
    let reg = /^\d+(\.\d+)?$/;
    let out = str.match(reg);
    if (!out) {
        return false;
    } else {
        return parseFloat(str);
    }
}

function buildNextNode(operations, nodes) {
    let op = operations.pop();
    let rhs = nodes.pop(); // reversed because it's a stack
    let lhs = nodes.pop();
    return buildNode(lhs, rhs, op);
}

function performOperation(num1, num2, op) {
    if (op === '+')
        return num1 + num2;
    else if (op === '-')
        return num1 - num2;
    else if (op === '*')
        return num1 * num2;
    else if (op === '/')
        return num1 / num2;
    else 
        throw 'Invalid expression'
}

function buildNode(lhs, rhs, op) {
    return { calc : function() {
        return performOperation(lhs.calc(), rhs.calc(), op);
    }}
}

function buildSimpleNode(str) {
    return { calc : function() {
        return evaluate(str);
    }};
}

function parse(chunk) {
    if (chunk.length == 0)
        throw 'Invalid expression'
    let operations = [];
    let nodes = [];
    for (let i = 0; i < chunk.length - 1; i++) {
        let ch = chunk[i];
        if (!isOperation(ch)) {
            let str = '';
            while (!isOperation(ch) && i < chunk.length - 1) { 
                str += ch;
                i++;
                ch = chunk[i];
            }
            i--;
            let num = buildSimpleNode(str);
            nodes.push(num)
        } else {
            while (operations.length > 0 && getPriority(ch) < getPriority(operations.slice(-1)[0])) {
                nodes.push(buildNextNode(operations, nodes));
            }
            operations.push(ch);
        }
    }
    while (operations.length > 0) {
        nodes.push(buildNextNode(operations, nodes));
    }
    return nodes.pop();
}

function addMapping(str) {
    let x = str.split('=')
    let a = x[0]
    let b = x[1]
    mappings[a] = parseFloat(b);
    console.log('added ' + a + '=' + b);
}

process.stdin.on('data', (chunk) => {
    if (chunk[0] === 'c') {
        console.log(tree.calc());
    } else if (chunk[0] === 'a') {
        tree = addMapping(chunk.substring(2, chunk.length));
    } else if (chunk[0] === 'e') {
        tree = parse(chunk.substring(2, chunk.length));
    }
});
