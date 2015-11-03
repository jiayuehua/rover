#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
namespace{
namespace msm = boost::msm;
namespace mpl = boost::mpl;

// events
typedef std::pair<int,int> Point;
Point down(1,0);
Point up(-1,0);
Point right(0,1);
Point left(0,-1);
Point downleft(1,-1);
Point upright(-1,1);
struct play {};

// front-end: define the FSM structure 
struct Timerotate1q_Machine : public msm::front::state_machine_def<Timerotate1q_Machine>
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
    struct Bottom: public msm::front::state<> 
    {
        // every (optional) entry/exit methods get the event passed.
        template <class Event,class FSM>
        void on_entry(Event const&,FSM& ) {}
        template <class Event,class FSM>
        void on_exit(Event const&,FSM& ) {}
    };
    struct Top : public msm::front::state<> 
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
    struct End: public msm::front::state<> 
	{};
  
    // the initial state of the player SM. Must be defined
    typedef Top initial_state;
    typedef Timerotate1q_Machine p; // makes transition table cleaner
private:
	int x_, y_;
	int LX_;
	int LY_;
	Point direction_ ;
public:
	Timerotate1q_Machine(int x)       :x_(0),y_(x-1), LX_(x), LY_(x) ,direction_ (down){
	}
	Timerotate1q_Machine(int x, int y):x_(0),y_(y-1), LX_(x), LY_(y) ,direction_(down){
	}
	void print()const{
		//std::cout<<"("<<x_<<","<<y_<<")";
	}
	Point get()const
	{
		return std::make_pair(x_,y_);
	}
	int get(int)const
	{
		return x_* LX_ + y_;
	}
	bool touch_bottom_border(play const&)
	{
		if (x_+ direction_.first == LX_ -1)
		{
			return true;
		}
		return false;
	}

	bool not_touch_bottom_border(play const&p1)
	{
		return ! touch_bottom_border(p1);
	}
	bool inside_left_border(play const&)
	{
		if (y_  ==0)
		{
			return true;
		}
		return false;
	}

	bool not_inside_left_border(play const&p1)
	{
		return ! inside_left_border(p1);
	}
    void forward_fun(play const&p1)     {
		x_+=direction_.first;
		y_+=direction_.second ;
	} 
    void smooth_fun(play const&p1)     { forward_fun(p1) ;print();} 
	void prev_column(play const& p1)
	{
		x_ = 0;
		--y_;
		print();
	}
	bool one_row(play const& p1)
	{
		return LX_ == 1;
	}
	bool one_row_touchbottom(play const& p1)
	{
		return one_row(p1) && touch_bottom_border(p1);
	}

    // Transition table for player
    struct transition_table : mpl::vector<
        //  +---------+-------------+---------+---------------------+----------------------------+
        //    Start     Event         Next      Action				 Guard
        //  +---------+-------------+---------+---------------------+----------------------------+
		row < Top	  , play        , Top     ,	&p::smooth_fun      , &p::not_touch_bottom_border>,
		row < Top     , play		, Bottom  , &p::smooth_fun		, &p::touch_bottom_border    >, 
		row < Top     , play		, Top     , &p::prev_column     , &p::one_row>, 
		row < Top     , play		, End     , &p::prev_column     , &p::one_row_touchbottom>, 
        //  +---------+-------------+---------+---------------------+----------------------------+
		row < Bottom  , play		, Top     , &p::prev_column     , &p::not_inside_left_border  >,
		row < Bottom  , play		, End     , &p::prev_column     , &p::inside_left_border  >
        //  +---------+-------------+---------+---------------------+----------------------------+
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
typedef msm::back::state_machine<Timerotate1q_Machine> player;
//
// Testing utilities.
//
static char const* const state_names[] = { "Top",  "Bottom" ,"End"};
void pstate(player const& p)
{
	std::cout<<p.current_state()[0];
    std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
}

void test()
{        
	const int N = 5;
	int a[N][N];
	for (int i = 0; i<N; ++i)
	{
		for (int j = 0; j<N; ++j)
		{
			a[i][j] = i;
			std::cout<<a[i][j]<<" ";
		}
		std::cout<<"\n";
	}
	player p(N,N);
    // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
    p.start(); 
	int n = N*N;
	int *parray = a[0];
	int j = 0;
	for (; p.current_state()[0] != 2 ; ++j)
	{
		std::cout<<parray[p.get(0)];
		if ((j+1)%5 == 0 )
		{
			std::cout<<std::endl;
		}
		p.process_event(play());

	}

    std::cout << "stop fsm" << std::endl;
    //p.stop();
}
}

int timer1q_test_main()
{
    test();
    return 0;
}
