#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/modulus.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/advance.hpp>
namespace{
namespace msm = boost::msm;
namespace mpl = boost::mpl;

typedef std::pair<int,int> Point;
Point down(1,0);
Point up(-1,0);
Point right(0,1);
Point left(0,-1);
Point downleft(1,-1);
Point upright(-1,1);
Point quarters[] = {right, down, left, up};

// events
struct play {};

template <class T>
void print(const T & s)
{
	//std::cout<<s<<std::endl;
}

// The list of FStateMachine states
struct Normal : public msm::front::state<> 
{
    // every (optional) entry/exit methods get the event passed.
    template <class Event,class FStateMachine>
    void on_entry(Event const&,FStateMachine& ) 
	{
        print( "entering: Normal" );
	}

    template <class Event,class FStateMachine>
    void on_exit(Event const&,FStateMachine& ) {
        print( "leaving: Normal" );
	}
};
struct Top : public msm::front::state<> 
{

    // every (optional) entry/exit methods get the event passed.
    template <class Event,class FStateMachine>
    void on_entry(Event const&,FStateMachine& fsm) 
	{
        print( "entering: Top" );
	}
    template <class Event,class FStateMachine>
    void on_exit(Event const&,FStateMachine&fsm ) {
        print( "leaving: Top" );
	}
};
struct Left : public msm::front::state<> 
{
    // every (optional) entry/exit methods get the event passed.
    template <class Event,class FStateMachine>
    void on_entry(Event const&,FStateMachine&fsm) 
	{
        print( "entering: Left" );
	}
    template <class Event,class FStateMachine>
    void on_exit(Event const&,FStateMachine& fsm)
	{
        print( "leaving: Left" );
	}
};
struct Bottom: public msm::front::state<> {
    // every (optional) entry/exit methods get the event passed.
    template <class Event,class FStateMachine>
    void on_entry(Event const&,FStateMachine& fsm ) 
	{
        print( "entering: Bottom" );
	}
    template <class Event,class FStateMachine>
    void on_exit(Event const&,FStateMachine& fsm) {
        print( "leaving: Bottom" );
	}
};
struct Right: public msm::front::state<> {
    // every (optional) entry/exit methods get the event passed.
    template <class Event,class FStateMachine>
    void on_entry(Event const&,FStateMachine& fsm) 
	{
        print( "entering: Right" );
	}
    template <class Event,class FStateMachine>
    void on_exit(Event const&,FStateMachine& fsm ) {
        print( "leaving: Right" );
	}
};
struct End: public msm::front::state<> {
    // every (optional) entry/exit methods get the event passed.
    template <class Event,class FStateMachine>
    void on_entry(Event const&,FStateMachine& ) 
	{
        print( "entering: End" );
	}
    template <class Event,class FStateMachine>
    void on_exit(Event const&,FStateMachine& ) {
        print( "leaving: End" );
	}
};
struct Last: public msm::front::state<> {
    // every (optional) entry/exit methods get the event passed.
    template <class Event,class FStateMachine>
    void on_entry(Event const&,FStateMachine&fsm ) 
	{
        print( "entering: Last" );
		//fsm.change_direction(right);
	}
    template <class Event,class FStateMachine>
    void on_exit(Event const&,FStateMachine& ) {
        print( "leaving: Last" );
	}
};
typedef mpl::vector<Left, Top, Right, Bottom> Sides;
template<class State>
struct State_Index_NextState_Traits
{
private:
	typedef mpl::find<Sides, Bottom>::type It1;
	typedef mpl::begin<Sides>::type ItB;
	typedef mpl::distance<ItB, It1>::type index;
	typedef mpl::plus<mpl::int_<2>, index>::type opposite_index_imp;
	typedef mpl::modulus<opposite_index_imp,mpl::int_<3> >::type opposite_index;
public:
	enum{ pointsindex = index::value};
	typedef mpl::at<Sides,opposite_index>::type Next_state;
};

template<class State,  int LX, int LY>
struct State_Action_Traits;
template<int LX, int LY>
struct State_Action_Traits<Bottom, LX, LY>
{
public:
	enum {OriginX = LX - 1, OriginY = 0};
	static	void another_col_or_row(int * x, int * y)
	{
		if (x&&y)
		{
			++(*y);
			*x = LX - 1;
		}
	}

	static	bool one_row_or_one_col()
	{
		return LX == 1;
	}

	static bool touch_opposite_border(int *px, int* py, const Point & direction)
	{
		if (px &&( (*px) + direction.first == 0) )
		{
			return true;
		}
		return false;
	}

	static bool inside_reversetimer_border(int* px, int* py)
	{
		if (py && (*py == LY - 1))
		{
			return true;
		}
		return false;
	}
};

// front-end: define the FStateMachine structure 
template <class Initial_state, int LX1, int LY1>
struct Timerotate3q_Machine : public msm::front::state_machine_def<Timerotate3q_Machine<Initial_state, LX1, LY1> >
{

    template <class Event,class FStateMachine>
    void on_entry(Event const& ,FStateMachine&) 
    {
        std::cout << "entering: Player" << std::endl;
    }

    template <class Event,class FStateMachine>
    void on_exit(Event const&,FStateMachine& ) 
    {
        std::cout << "leaving: Player" << std::endl;
    }

    // the initial state of the player StateMachine. Must be defined
    typedef Initial_state initial_state;
    typedef Timerotate3q_Machine p; // makes transition table cleaner

private:
	typedef State_Action_Traits<initial_state, LX1, LY1> State_action_traits;
	int x_, y_;
	Point direction_ ;

public:
	Timerotate3q_Machine() : x_(State_action_traits::OriginX), y_(State_action_traits::OriginY), direction_(quarters[State_Index_NextState_Traits<initial_state>::pointsindex]) {}
	void print()const{
		//std::cout<<"("<<x_<<","<<y_<<")";
	}

	Point get()const
	{
		return std::make_pair(x_,y_);
	}

	int get(int)const
	{
		return x_* LX1 + y_;
	}

	bool touch_opposite_border(play const&)
	{
		return 	State_action_traits::touch_opposite_border(&x_, &y_, direction_);
	}
	bool not_touch_opposite_border(play const&p1)
	{
		return ! touch_opposite_border(p1);
	}

	bool inside_reversetimer_border(play const&)
	{
		State_action_traits::inside_reversetimer_border(&x_, &y_);
		return false;
	}

	bool not_inside_reversetimer_border(play const&p1)
	{
		return ! inside_reversetimer_border(p1);
	}

    void smooth_fun(play const&p1)     {
		x_+=direction_.first;
		y_+=direction_.second ;
	} 

	void another_col_or_row(play const& p1)
	{
		State_action_traits::another_col_or_row(&x_, &y_);
	}

	bool one_row_or_one_col(play const& p1)
	{
		return State_action_traits::one_row_or_one_col();
	}
	
	bool one_row_one_col_touch_opposite_border(play const& p1)
	{
		return one_row_or_one_col(p1)&&touch_opposite_border(p1);
	}

	typedef typename State_Index_NextState_Traits<initial_state>::Next_state Next_state;
	//bool touch_bottom_border(play const&)
	//{
	//	if (x_+ direction_.first == LX1 -1)
	//	{
	//		return true;
	//	}
	//	return false;
	//}
	//bool not_touch_bottom_border(play const&p1)
	//{
	//	return ! touch_bottom_border(p1);
	//}
	//bool touch_left_border(play const&)
	//{
	//	if (y_+ direction_.second == 0)
	//	{
	//		return true;
	//	}
	//	return false;
	//}
	//bool not_touch_left_border(play const&p1)
	//{
	//	return ! touch_left_border(p1);
	//}
	//bool inside_top_border(play const&)
	//{
	//	if (x_  ==0)
	//	{
	//		return true;
	//	}
	//	return false;
	//}
	//bool not_inside_top_border(play const&p1)
	//{
	//	return ! inside_top_border(p1);
	//}
	//bool inside_left_border(play const&)
	//{
	//	if (y_  ==0)
	//	{
	//		return true;
	//	}
	//	return false;
	//}
	//bool not_inside_left_border(play const&p1)
	//{
	//	return ! inside_left_border(p1);
	//}
	//void prev_column(play const& p1)
	//{
	//	--y_;
	//	x_ = LX1 -1;
	//	print();
	//}
	//void prev_row(play const& p1)
	//{
	//	--x_;
	//	y_ = LY1 -1;
	//	print();
	//}
	//bool one_col_topleft(play const& p1)
	//{
	//	return one_col(p1)&&inside_top_border(p1);
	//}
	//bool one_col(play const& p1)
	//{
	//	return LY1 == 1;
	//}

	// Transition table for player
	struct transition_table : mpl::vector<
		//  +----------------+----------+----------------+--------------------------+-------------------------------------------+
		//  + Start			   Event      Next			   Action					  Guard										+
		//  +----------------+----------+----------------+--------------------------+-------------------------------------------+
		row < initial_state  , play     , initial_state  , &p::smooth_fun           , &p::not_touch_opposite_border	   	  	  	>,
		row < initial_state  , play		, Next_state     , &p::smooth_fun		    , &p::touch_opposite_border			 	    >, 
		row < initial_state  , play		, initial_state  , &p::another_col_or_row   , &p::one_row_or_one_col			    	>, 
		row < initial_state  , play		, End			 , &p::another_col_or_row   , &p::one_row_one_col_touch_opposite_border	>, 
		//  +----------------+----------+----------------+--------------------------+-------------------------------------------+
		row < Next_state     , play		, initial_state  , &p::another_col_or_row   , &p::not_inside_reversetimer_border        >,
		row < Next_state     , play		, End            , &p::another_col_or_row   , &p::inside_reversetimer_border            >
		//  +----------------+----------+----------------+--------------------------+-------------------------------------------+
	> {};

    // Replaces the default no-transition response.
    template <class FStateMachine,class Event>
    void no_transition(Event const& e, FStateMachine&,int state)
    {
        std::cout << "no transition from state " << state
            << " on event " << typeid(e).name() << std::endl;
    }
};
// Pick a back-end
typedef msm::back::state_machine<Timerotate3q_Machine<Bottom, 5, 5> > player;
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
	player p;
    // needed to start the highest-level StateMachine. This will call on_entry and mark the start of the StateMachine
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

int time4quarter_main()
{
    test();
    return 0;
}