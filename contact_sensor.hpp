/*
 * contact_sensor.hpp
 *
 *  Created on: Oct 3, 2025
 *      Author: Oleksiy
 */

#ifndef NIKOLEX_CONTACT_SENSOR_CONTACT_SENSOR_HPP_
#define NIKOLEX_CONTACT_SENSOR_CONTACT_SENSOR_HPP_

#include <cstdint>
#include "../interfaces/reader.hpp"
#include "../interfaces/input_pin.hpp"

namespace nikolex {

class ContactSensor: public Reader{

public:
	enum State{
		CLOSED,
		OPENED
	};

private:

	State state;
	bool _isNormalOpenHigh;
	InputPin* _pin;

	void(* onOpenCallback)(void*) = nullptr;
	void(* onCloseCallback)(void*) = nullptr;
	void(* onChangeCallback)(State, void*) = nullptr;
	void(* onReadCloseCallback)(void*) = nullptr;
	void(* onReadOpenCallback)(void*) = nullptr;
	void(* onReadCallback)(State, void*) = nullptr;

	void* onOpenCtx = nullptr;
	void* onCloseCtx = nullptr;
	void* onChangeCtx = nullptr;
	void* onReadCloseCtx = nullptr;
	void* onReadOpenCtx = nullptr;
	void* onReadCtx = nullptr;



	void setState(State st){
		State prevState = state;
		state = st;
		if(onReadCallback) onReadCallback(state, onReadCtx);
		if(state == State::OPENED){
			if(onReadOpenCallback) onReadOpenCallback(onReadOpenCtx);
		}
		else{
			if(onReadCloseCallback) onReadCloseCallback(onReadCloseCtx);
		}
		if(prevState != state){
			if(onChangeCallback) onChangeCallback(state, onChangeCtx);
			if(state == State::OPENED){
				if(onOpenCallback) onOpenCallback(onOpenCtx);
			}
			else{
				if(onCloseCallback) onCloseCallback(onCloseCtx);
			}
		}
	}

	void setOpen(){
		setState(State::OPENED);
	}


	void setClose(){
		setState(State::CLOSED);
	}

public:

	ContactSensor(InputPin* pin, bool isNormalOpenHigh = true): _isNormalOpenHigh(isNormalOpenHigh), _pin(pin){

	};



	void onOpen(void(* callback)(void*), void* ctx = nullptr){
		onOpenCallback = callback;
		onOpenCtx = ctx;
	}

	void onClose(void(* callback)(void*), void* ctx = nullptr){
		onCloseCallback = callback;
		onCloseCtx = ctx;
	}

	void onChange(void(* callback)(State state, void*), void* ctx = nullptr){
		onChangeCallback = callback;
		onChangeCtx = ctx;
	}

	virtual void onReadClose(void(* callback)(void*), void* ctx = nullptr) final{
		onReadCloseCallback = callback;
		onReadCloseCtx = ctx;
	}

	void onReadOpen(void(* callback)(void*), void* ctx = nullptr){
		onReadOpenCallback = callback;
		onReadOpenCtx = ctx;
	}

	void onRead(void(* callback)(State state, void*), void* ctx = nullptr){
		onReadCallback = callback;
		onReadCtx = ctx;
	}


	virtual void read() override{
		InputPin::State pinState = _pin->read();
		if(_isNormalOpenHigh){
			if(pinState == InputPin::State::HIGH) setOpen();
			else(setClose());
		}
		else{
			if(pinState == InputPin::State::LOW) setOpen();
			else(setClose());
		}
	}

	State getState(){
		return state;
	}
};

}



#endif /* NIKOLEX_CONTACT_SENSOR_CONTACT_SENSOR_HPP_ */
