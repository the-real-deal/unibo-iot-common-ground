#include "StateHolder.hpp"

template <typename S>
StateHolder<S>::StateHolder(S initialState) {
    setState(initialState);
}

template <typename S>
S StateHolder<S>::getState() {
    return state;
}

template <typename S>
void StateHolder<S>::setState(S newState) {
    this->state = newState;
}