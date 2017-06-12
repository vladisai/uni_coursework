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
    Dimensions,
} from 'react-native';

class CircleComponent extends Component {
    
    constructor(props) {
        super(props);
        this.state = {isTapped: false, c: randomHole()};
    }

    componentWillReceiveProps(nextProps) {
        if (this.props.topOffset === nextProps.topOffset) {
            return;
        }
        state = this.state;
        state.isTapped = false; 
        state.c = randomHole();
        this.setState(state);
    }

    render() {
        return (
            <TouchableOpacity onPress={() => this.onPress()} style={[{position: 'absolute', left: this.state.c.offset, top: this.props.topOffset}]}>
                        <View style={[styles.circle]}>
                { (this.state.c.character !== 'none') ? 
                        <Image style={[this.opacityStyle(), {width: 80, height: 80}]} source={this.state.c.character}/>
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
        return dict;
    }
    
    onPress() {
        state = this.state
        if (!state.isTapped) {
            this.props.callback(this.state.c.character);
        }
        state.isTapped = true;
        this.setState(state);
    }
}

const gameTime = 30000;
const timeout = 10;
const step = Dimensions.get('window').width / (gameTime / timeout);

export default class HelloWorldRn extends Component {
    constructor(props) {
        super(props);
        this.state = this.generateState();
        this.timeoutF = this.timeoutF.bind(this);
        this.restartGame = this.restartGame.bind(this);
        setTimeout(this.timeoutF, timeout);
    }

    timeoutF() {
        let state = this.state;
        if (state.progressBarWidth <= 0) {
            let score = this.state.aliensKilled - this.state.astronautsKilled;
            let title = 'Game over';
            let text = 'Your score is ' + score;
            let buttons = [{text: 'Reset', onPress: this.restartGame}];
            Alert.alert(title, text, buttons, {cancelable: false});
        } else {
            let currentDate = new Date();
            let diff = currentDate - state.gameStartDate;
            let progress = 1 - diff / gameTime;
            let totalWidth = Dimensions.get('window').width;
            state.progressBarWidth = progress * totalWidth;
            if (progress < 0.5) {
                state.progressBarColor = 'orange';
            }
            if (progress < 0.25) {
                state.progressBarColor = 'red';
            }
            this.setState(state);
            setTimeout(this.timeoutF, timeout);
        }
    }

    render() {
        return (
            <View style={styles.container}>
                <CircleComponent callback={(a) => this.onCircleTouched(a)} topOffset={this.state.offset_0}/>
                <CircleComponent callback={(a) => this.onCircleTouched(a)} topOffset={this.state.offset_1}/>
                <CircleComponent callback={(a) => this.onCircleTouched(a)} topOffset={this.state.offset_2}/>
                <CircleComponent callback={(a) => this.onCircleTouched(a)} topOffset={this.state.offset_3}/>
                <Text>Aliens killed: {this.state.aliensKilled}</Text>
                <Text>Astronauts killed: {this.state.astronautsKilled}</Text>
                <View style={[{width: this.state.progressBarWidth, height:20, backgroundColor: this.state.progressBarColor}]}/>
                <Button title="Reset" onPress={() => this.onPress()}/>
            </View>
        );
    }

    onCircleTouched(val) {
        if (val === astronaut) {
            state = this.state;
            state.astronautsKilled++;
            this.setState(state);
        } else if (val === alien) {
            state = this.state
            state.aliensKilled++;
            this.setState(state);
        }
    }

    generateCircle(a) {
        b = a + randomIntegerBetween(-10, 10);
        e = (
                <CircleComponent topOffset={b}/>
        );
        return e;
    }

    generateState() {
        state = this.state;
        if (!state) {
            state = {aliensKilled: 0,
                    astronautsKilled: 0,
                    progressBarWidth: Dimensions.get('window').width,
                    progressBarColor: 'green',
                    gameStartDate: new Date()};
        }
        height = Dimensions.get('window').height;
        let current = height / 6;
        let step = height / 6;
        state.offset_0 = current + randomIntegerBetween(-10, 10);
        current += step;
        state.offset_1 = current + randomIntegerBetween(-10, 10);
        current += step;
        state.offset_2 = current + randomIntegerBetween(-10, 10);
        current += step;
        state.offset_3 = current + randomIntegerBetween(-10, 10);
        return state;
    }

    onPress() {
        this.setState(this.generateState());
    }

    restartGame() {
        delete this.state;
        this.setState(this.generateState());
        setTimeout(this.timeoutF, timeout);
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
    },
});

let randomIntegerBetween = (min, max) => {
    return Math.floor(Math.random() * (max - min)) + min;
}

let randomElementOf = (items) => {
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
