//
// Created by valeriy on 17.05.17.
//

#ifndef FUTUREANDPROMISE_STATE_H
#define FUTUREANDPROMISE_STATE_H

template<typename T>
class State {
public:
    State( ) : isValueSet( false ), isExceptionCatched( false ) { }

    bool getIsValueSet( ) {
        return this->isValueSet;
    }

    void setValue( T value ) {
        this->value = value;
        this->isValueSet = true;
    }

    T getValue() {
        return this->value;
    }

    bool getIsExceptionCatched( ) {
        return this->isExceptionCatched;
    }

    void catchException( std::exception ex ) {
        this->exception = ex;
        this->isValueSet = true;
        this->isExceptionCatched = true;
    }

    std::exception getException() {
        return this->exception;
    }

private:
    bool isValueSet;
    bool isExceptionCatched;
    T value;
    std::exception exception;
};


#endif //FUTUREANDPROMISE_STATE_H
