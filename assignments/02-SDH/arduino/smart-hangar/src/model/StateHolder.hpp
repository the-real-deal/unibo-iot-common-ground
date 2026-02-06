template <typename S>
class StateHolder {
    private:
        S state;
    public:
        StateHolder(S initialState);
        S getState();
        void setState(S newState);  
};