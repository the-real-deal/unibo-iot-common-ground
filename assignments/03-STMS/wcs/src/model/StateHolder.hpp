#ifndef __STATE_HOLDER__
#define __STATE_HOLDER__

template <typename S>
class StateHolder
{
private:
    S state;

public:
    StateHolder(S initialState)
    {
        setState(initialState);
    }

    S getState()
    {
        return state;
    }

    void setState(S newState)
    {
        this->state = newState;
    }
};

#endif