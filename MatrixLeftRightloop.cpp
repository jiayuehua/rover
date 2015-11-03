
#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
namespace{
namespace msm = boost::msm;
namespace mpl = boost::mpl;

// events
int x = 0; int y = 0;
typedef std::pair<int,int> Point;
std::pair<int,int> down(1,0);
std::pair<int,int> right(0,1);
std::pair<int,int> left(0,-1);
std::pair<int,int> downleft(1,-1);
std::pair<int,int> upright(-1,1);
struct play {};

// front-end: define the FSM structure 
struct RotateMatrix_Machine : public msm::front::state_machine_def<RotateMatrix_Machine>
{
    template <class Event,class FSM>
    void on_entry(Event const& ,FSM&) 
    {
        std::cout << "entering: Player" << std::endl;
    }
    template <class Event,class FSM>
    void on_exit(Event const&,FSM& ) 
    {
        std::cout << "leaving: Player" << std::endl;
    }

    // The list of FSM states
    struct Normal : public msm::front::state<> 
    {
        // every (optional) entry/exit methods get the event passed.
        template <class Event,class FSM>
        void on_entry(Event const&,FSM& ) {}
        template <class Event,class FSM>
        void on_exit(Event const&,FSM& ) {}
    };
    struct Left : public msm::front::state<> 
    {
        // every (optional) entry/exit methods get the event passed.
        template <class Event,class FSM>
        void on_entry(Event const&,FSM& ) {}
        template <class Event,class FSM>
        void on_exit(Event const&,FSM& ) {}
    };
    struct Right: public msm::front::state<> 
    {
        // every (optional) entry/exit methods get the event passed.
        template <class Event,class FSM>
        void on_entry(Event const&,FSM& ) {}
        template <class Event,class FSM>
        void on_exit(Event const&,FSM& ) {}
    };
  
    // the initial state of the player SM. Must be defined
    typedef Left initial_state;


    typedef RotateMatrix_Machine p; // makes transition table cleaner
private:
	bool first_touch_border_;
	int x_, y_;
	int LX_;
	int LY_;
Point direction ;//= down;
public:
	RotateMatrix_Machine(int y):first_touch_border_(true),x_(0),y_(0), LY_(y){
		LX_ = 3;
		Point direction = down;
	}
	bool first_touch_border(const play&)
	{
		return first_touch_border_;
	}
	bool inside_border(const play&p)
	{
		return ! first_touch_border(p);
	}
	void print()const{
		std::cout<<"("<<x_<<","<<y_<<")";
	}
	bool touch_right_border(play const&)
	{
		if (y_+ direction.second == LY_ -1)
		{
			return true;
		}
		return false;
	}

	bool touch_left_border(play const&)
	{
		if (y_ +direction.second ==0)
		{
			return true;
		}
		return false;
	}
	bool not_touch_border(play const&p)
	{
		return ! touch_left_border(p) && ! touch_right_border(p);
	}
    void forward_fun(play const&p1)     { x_+=direction.first; y_+=direction.second ;} 
	void touch_right_fun(play const&p1){first_touch_border_ = true; smooth_fun(p1);}
	void touch_left_fun(play const&p1){first_touch_border_ = true; smooth_fun(p1);}
	void down_fun(play const&p1)       { std::cout<<std::endl;direction = down; first_touch_border_ = false;forward_fun(p1);print();}
    void right_fun(play const&p1)     { direction = right ; forward_fun(p1);print();} 
    void left_fun(play const&p1)     { direction = left ;forward_fun(p1);print();} 
    void smooth_fun(play const&p1)     { forward_fun(p1) ;print();} 
    // Transition table for player
    struct transition_table : mpl::vector<
        //    Start     Event         Next      Action				 Guard
        //  +---------+-------------+---------+---------------------+----------------------+
		row < Left	  , play        , Left    ,	&p::down_fun        ,&p::first_touch_border>,
		row < Left    , play		, Normal  , &p::right_fun     ,&p::inside_border     >, 
		row < Normal    , play		, Normal  , &p::smooth_fun     ,&p::not_touch_border      >,
		row < Normal    , play		, Right  , &p::touch_right_fun     ,&p::touch_right_border      >,
		row < Normal    , play		, Left  , &p::touch_left_fun     ,&p::touch_left_border      >,
		row < Right    , play		, Right  , &p::down_fun     ,&p::first_touch_border      >,
		row < Right    , play		, Normal  , &p::left_fun     ,&p::inside_border      >
      
    > {};
    // Replaces the default no-transition response.
    template <class FSM,class Event>
    void no_transition(Event const& e, FSM&,int state)
    {
        std::cout << "no transition from state " << state
            << " on event " << typeid(e).name() << std::endl;
    }
};
// Pick a back-end
typedef msm::back::state_machine<RotateMatrix_Machine> player;

//
// 
//
static char const* const state_names[] = { "Left", "Normal", "Right" };
void pstate(player const& p)
{
    std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
}

void left_right_rover()
{        
	player p(5);
    // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
  p.start(); 
	int n = 100;
	for (int i = 0; i<n;++i)
	{
		p.process_event(play());pstate(p);
	}
    std::cout << "stop fsm" << std::endl;
    //p.stop();
}
}

int main()
{
    left_right_rover();
    return 0;
}