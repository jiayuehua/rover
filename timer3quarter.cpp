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
struct Timerotate3q_Machine : public msm::front::state_machine_def<Timerotate3q_Machine>
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
    typedef Bottom initial_state;
    typedef Timerotate3q_Machine p; // makes transition table cleaner
private:
	bool first_touch_border_;
	int x_, y_;
	int LX_;
	int LY_;
	Point direction_ ;
public:
	Timerotate3q_Machine(int x)       :x_(x-1),y_(0), LX_(x), LY_(x) ,direction_(up){
	}
	Timerotate3q_Machine(int x, int y):x_(x-1),y_(0), LX_(x), LY_(y) ,direction_(up){
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
	bool touch_left_border(play const&)
	{
		if (y_+ direction_.second == 0)
		{
			return true;
		}
		return false;
	}

	bool not_touch_left_border(play const&p1)
	{
		return ! touch_left_border(p1);
	}
	bool touch_top_border(play const&)
	{
		if (x_+ direction_.first == 0)
		{
			return true;
		}
		return false;
	}
	bool not_touch_top_border(play const&p1)
	{
		return ! touch_top_border(p1);
	}

	bool inside_top_border(play const&)
	{
		if (x_  ==0)
		{
			return true;
		}
		return false;
	}

	bool not_inside_top_border(play const&p1)
	{
		return ! inside_top_border(p1);
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
	bool inside_reversetimer_border(play const&)
	{
		if (y_  == LY_ -1)
		{
			return true;
		}
		return false;
	}

	bool not_inside_right_border(play const&p1)
	{
		return ! inside_reversetimer_border(p1);
	}
    void forward_fun(play const&p1)     {
		x_+=direction_.first;
		y_+=direction_.second ;
	} 
    void smooth_fun(play const&p1)     { forward_fun(p1) ;print();} 
	void prev_column(play const& p1)
	{
		--y_;
		x_ = LX_ -1;
		print();
	}
	void next_col(play const& p1)
	{
		++y_;
		x_ = LX_ -1;
		print();
	}

	void prev_row(play const& p1)
	{
		--x_;
		y_ = LY_ -1;
		print();
	}
	bool one_col_topleft(play const& p1)
	{
		return one_col(p1)&&inside_top_border(p1);
	}
	bool one_col(play const& p1)
	{
		return LY_ == 1;
	}
	bool one_row(play const& p1)
	{
		return LX_ == 1;
	}
	bool one_row_touchtop(play const& p1)
	{
		return one_row(p1)&&touch_top_border(p1);
	}

    // Transition table for player
    struct transition_table : mpl::vector<
        //  +---------+-------------+---------+---------------------+----------------------------+
        //    Start     Event         Next      Action				 Guard
        //  +---------+-------------+---------+---------------------+----------------------------+
		row < Bottom  , play        , Bottom  ,	&p::smooth_fun      , &p::not_touch_top_border	 >,
		row < Bottom  , play		, Top     , &p::smooth_fun		, &p::touch_top_border		 >, 
		row < Bottom  , play		, Bottom  , &p::next_col        , &p::one_row				 >, 
		row < Bottom  , play		, End	  , &p::next_col        , &p::one_row_touchtop		 >, 
        //  +---------+-------------+---------+---------------------+----------------------------+
		row < Top      , play		, Bottom  , &p::next_col        , &p::not_inside_right_border>,
		row < Top      , play		, End     , &p::next_col        , &p::inside_reversetimer_border    >
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
typedef msm::back::state_machine<Timerotate3q_Machine> player;
//
// Testing utilities.
//
static char const* const state_names[] = { "Bottom", "Top", "End"};
void pstate(player const& p)
{
	std::cout<<p.current_state()[0];
    std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
}

void test()
{        
	const int N = 5;
	int a[N][N];
	int k = -1;
	for (int i = 0; i<N; ++i)
	{
		for (int j = 0; j<N; ++j)
		{
			a[i][j] = ++k;
			std::cout<<a[i][j]<<" ";
		}
		std::cout<<"\n";
	}
	player p(N,N);
    // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
    p.start(); 
	int n = N*N;
	int *parray = a[0];
	for (int j = 0; j!=N*N ; ++j)
	{
		//pstate(p);

		std::cout<<parray[p.get(0)]<<" ";
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

int  time3qtest_main()
{
    test();
    return 0;
}