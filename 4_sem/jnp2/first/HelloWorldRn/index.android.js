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
    Alert,
    View,
    Image,
    Button,
    TouchableOpacity,
} from 'react-native';

class CircleComponent extends Component {
    
    constructor(props) {
        super(props);
        this.state = {isTapped: false};
        this.c = randomHole();
        this.topOffset = this.props.topOffset;
    }

    render() {
        return (
            <TouchableOpacity onPress={() => {this.onPress() }}>
                <View style={[styles.circle, {left: this.c.offset, top: this.topOffset}]}>
                { (this.c.character !== 'none') ? 
                        <Image style={[this.opacityStyle(), {width: 80, height: 80}]} source={this.c.character}/>
                 : null }
                </View>
            </TouchableOpacity>
        );
    }

    opacityStyle() {
        dict = {
            opacity: 1,
        }
        if (this.state.isTapped)
            dict.opacity = 0 
        return dict
    }
    
    onPress() {
        this.setState({isTapped: true})
    }
}

                    // <Image style={{width: 80, height: 80}} source={require('./alien.png')}/>
export default class HelloWorldRn extends Component {
    constructor(props) {
        super(props);
    }

    render() {
        return (
            <View style={styles.container}>
                <CircleComponent topOffset={90} />
                <CircleComponent topOffset={190} />
                <CircleComponent topOffset={290} />
                <CircleComponent topOffset={390} />
                <Button title="Reset" onPress={() => this.onPress()}/>
            </View>
        );
    }

    onPress() {
        this.forceUpdate();
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
    for (int i = 0; i < 20; i++) {
        Alert.alert(randomIntegerBetween(0, 5).toString());
    }
    return items[randomIntegerBetween(0, items.length)];
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
