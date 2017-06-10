process.stdin.setEncoding('utf8');

let mapping = {};
let assign = /([a-zA-Z]+)=(.+)/;
let variable = /([a-zA-Z]+)/;
let number = /^[+-]?\d+(\.\d+)?$/;

function isOperation(ch) {
    let operations = ['+', '-', '*', '/', '(', ')'];
    return operations.indexOf(ch) > -1;
}

function getPriority(ch) {
    if (ch === '+')
        return 1;
    else if (ch === '-')
        return 1;
    else if (ch === '(')
        return 0;
    else
        return 2;
}

function getMapping(mapping, str) {
    if (!(variable.test(str)))
        throw 'Invalid expression';
    if (!(str in mapping))
        throw 'Undefined variable: ' + str;
    return mapping[str];
}

function evaluate(str, mapping) {
    if (str.length === 0)
        throw 'Invalid expression'
    if (number.test(str)) {
        return parseFloat(str);
    } else {
        return getMapping(mapping, str);
    }
}

function buildNextNode(operations, nodes) {
    let op = operations.pop();
    let rhs = nodes.pop(); // reversed because it's a stack
    let lhs = nodes.pop();
    if (typeof(rhs) === "undefined")
        throw 'Invalid expression'
    if (typeof(lhs) === "undefined")
        throw 'Invalid expression'
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
    return { evaluate : function(mapping) {
        return performOperation(lhs.evaluate(mapping), rhs.evaluate(mapping), op);
    }}
}

function buildSimpleNode(str) {
    return { evaluate : function(mapping) {
        return evaluate(str, mapping);
    }};
}

function parse(chunk) {
    if (chunk.length === 0)
        throw 'Invalid expression'
    let operations = [];
    let nodes = [];
    let unary = true;
    let current_unary = 1;
    for (let i = 0; i < chunk.length; i++) {
        let ch = chunk[i];
        if (!isOperation(ch)) {
            let str = '';
            while (!isOperation(ch) && i < chunk.length) {
                str += ch;
                i++;
                ch = chunk[i];
            }
            i--;
            let num = buildSimpleNode(str);
            if (unary && current_unary === -1) {
                let unary_node = buildSimpleNode('-1')
                let out = buildNode(unary_node, num, '*')
                nodes.push(out)
                current_unary = 1;
            } else {
                nodes.push(num)
            }
            unary = false;
        } else {
            if (ch === '(') {
                operations.push(ch)
                unary = true;
            } else if (ch === ')') {
                if (i === 0 || chunk[i - 1] === '(')
                    throw 'Invalid expression';
                while (operations[operations.length - 1] !== '(') {
                    nodes.push(buildNextNode(operations, nodes));
                }
                operations.pop()
                unary = false
            } else {
                if (unary) {
                    if (ch === '-') {
                        current_unary *= -1;
                    } else if (ch !== '+') {
                        throw 'Invalid expression';
                    }
                } else {
                    while (operations.length > 0 && getPriority(ch) <= getPriority(operations.slice(-1)[0])) {
                        nodes.push(buildNextNode(operations, nodes));
                    }
                    operations.push(ch);
                    unary = true;
                }
            }
        }
    }
    while (operations.length > 0) {
        nodes.push(buildNextNode(operations, nodes));
    }
    if (nodes.length !== 1) {
        throw 'Invalid expression';
    }
    return nodes.pop();
}

function addMapping(str) {
    let ar = str.match(assign);
    let a = ar[1];
    let b = ar[2];
    mapping[a] = parse(b).evaluate(mapping);
    return mapping[a];
}

process.stdin.on('data', (chunk) => {
    chunk = chunk.replace(/\s/g, '');
    //console.error(chunk)
    try {
        if (assign.test(chunk)) {
            console.log(addMapping(chunk));
        } else {
            console.log(parse(chunk).evaluate(mapping));
        }
    } catch (e) {
        console.log(e);
    }
});
