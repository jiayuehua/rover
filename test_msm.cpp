#include <iostream>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
namespace msm = boost::msm;
namespace mpl = boost::mpl;
namespace mfront = boost::msm::front;
namespace mback = boost::msm::back;
//event
namespace{
struct Open_Door {};

struct House:public mfront::state_machine_def<House>{
	struct Door_close:public mfront::state<>
	{
	};
	struct Door_opened:public mfront::state<>{
		template <class Entry, class MSM>
		void on_entry(const Entry&, MSM&)
		{
			std::cout<<"Door_opened::on_entry\n";
		}

		template <class Entry, class MSM>
		void on_exit(const Entry&, MSM&)
		{
			std::cout<<"Door_opened::on_exit\n";
		}
	};
	template <class Event,class FSM>
	void on_entry(Event const& ,FSM&) 
	{
		std::cout << "entering: House" << std::endl;
	}
	template <class Event,class FSM>
	void on_exit(Event const&,FSM& ) 
	{
		std::cout << "leaving: House" << std::endl;
	}
	void ring(const Open_Door&)
	{
		std::cout<<"House::ring\n";
	}

	typedef Door_close initial_state;
	struct transition_table: mpl::vector<
		a_row<Door_close, Open_Door, Door_opened, &House::ring>,
		a_row<Door_opened, Open_Door, Door_close, &House::ring>
	>
	{
	};
};
typedef mback::state_machine<House> HOUSE;
}
int test_msm_main()
{
	HOUSE h;
	h.start();
	h.process_event(Open_Door());
	h.process_event(Open_Door());
//	h.stop();
	return 0;
}