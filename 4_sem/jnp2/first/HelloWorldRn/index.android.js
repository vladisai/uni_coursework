/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import {
    AppRegistry,
    StyleSheet,
    Text,
    View,
    Image,
    Button,
} from 'react-native';

class CircleComponent extends Component {

    render() {
        c = randomHole()
        topOffset = parseInt(this.props.topOffset)
        return (
            <View style={[styles.circle, {left: c.offset, top: topOffset}]}>
            { (c.character !== 'None') ? 
                    <Image style={{width: 80, height: 80}} source={c.character}/>
             : null }
            </View>
        );
    }
}

                    // <Image style={{width: 80, height: 80}} source={require('./alien.png')}/>
export default class HelloWorldRn extends Component {
    render() {
        return (
            <View style={styles.container}>
                <CircleComponent topOffset='90' />
                <CircleComponent topOffset='190' />
                <CircleComponent topOffset='290' />
                <CircleComponent topOffset='390' />
                <Button title="Reset"/>
            </View>
        );
    }
}
const styles = StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: '#F5FCFF',
    },
    circle: {
        borderWidth: 5,
        borderColor: '#808080',
        alignItems: 'center',
        width: 100,
        height: 100,
        borderRadius: 100/2,
        backgroundColor: '#0f0C0F',
        position: 'absolute',
    },
});

let randomIntegerBetween = (min, max) => {
    return Math.floor(Math.random() * (max - min)) + min;
}

let randomElementOf = (items) => {
    return items[randomIntegerBetween(0, items.length - 1)];
}

let astronaut = require('./astronaut.png')
let alien = require('./alien.png')

let randomHole = () => {
    return {
        offset: randomIntegerBetween(0, 200),
        character: randomElementOf([astronaut, alien, 'none'])
    }
}

AppRegistry.registerComponent('HelloWorldRn', () => HelloWorldRn);
