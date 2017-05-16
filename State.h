//
// Created by valeriy on 17.05.17.
//

#ifndef FUTUREANDPROMISE_STATE_H
#define FUTUREANDPROMISE_STATE_H

template<typename T>
class State {
public:
    State( ) : isValueSet( false ), isExceptionCatched( false ) { }

    bool isValueSet( ) {
        return this->isValueSet;
    }

    void setValue( T value ) {
        this->value = value;
        this->isValueSet = true;
    }

    T value() {
        return this->value;
    }

    bool isExceptionCatched( ) {
        return this->isExceptionCatched;
    }

    void catchException( std::exception ex ) {
        this->exception = ex;
        this->isExceptionCatched = true;
    }

    std::exception exception() {
        return this->exception;
    }

private:
    bool isValueSet;
    bool isExceptionCatched;
    T value;
    std::exception exception;
};


#endif //FUTUREANDPROMISE_STATE_H
