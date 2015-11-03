#include <iostream>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/row2.hpp>
#include <boost/msm/back/state_machine.hpp>
namespace msm = boost::msm;
namespace mfront = boost::msm::front;
namespace mback = boost::msm::back;
namespace mpl = boost::mpl;

//event
struct Push{};

//state machine
struct Door_:public mfront::state_machine_def<Door_>
{
	//state
	struct Opened:public mfront::state<>
	{
		template <class Event, class MSM>
		void on_entry(const Event&, MSM&)
		{
			std::cout<<"Opened::on_entry\n";

		}
		template <class Event, class MSM>
		void on_exit(const Event&, MSM&)
		{
			std::cout<<"Opened::on_exit\n";
		}
	};
	struct Closed:public mfront::state<>{};
		//action
	void sound(const Push&)
	{
		std::cout<<"Door_::sound\n";
		//transition_table
	}
	typedef Opened initial_state;
	typedef Door_ d;
	struct transition_table:public mpl::vector<
		a_row<Opened, Push,Closed,&d::sound>,
		a_row<Closed, Push,Opened,&d::sound>
	>{};
};
typedef mback::state_machine<Door_> Door;
int test1()
{
	Door d1;
	d1.start();
	d1.process_event(Push());
	d1.process_event(Push());
	d1.process_event(Push());
	d1.process_event(Push());
	d1.process_event(Push());
	d1.process_event(Push());
	d1.process_event(Push());
	return 0;
}
struct North { };
struct South { };
struct China:mfront::state_machine_def<China>{
	struct Beijing:mfront::state<>
	{
		template<class Event, class MSM>
		void on_entry(const Event&,  MSM&)
		{
			std::cout<<"Beijing::on_entry\n";
		}
		template<class Event, class MSM>
		void on_exit(const Event&,  MSM&)
		{
			std::cout<<"Beijing::on_exit\n";
		}
		void on_south(const South&)
		{
			std::cout<<"Beijing::on_south\n";
		}
	};

	struct Shanghai:mfront::state<>
	{
		template<class Event, class MSM>
		void on_entry(const Event&,  MSM&)
		{
			std::cout<<"Shanghai::on_entry\n";
		}
		template<class Event, class MSM>
		void on_exit(const Event&,  MSM&)
		{
			std::cout<<"Shanghai::on_exit\n";
		}
	};
		/*template<class Event, class MSM>
		void no_transition(const Event&,  MSM&, int state)
		{
			std::cout<<"Guangzhou::on_entry\n";
		}*/
	struct Guangzhou:mfront::state<>
	{
		template<class Event, class MSM>
		void on_entry(const Event&,  MSM&)
		{
			std::cout<<"Guangzhou::on_entry\n";
		}
		template<class Event, class MSM>
		void on_exit(const Event&,  MSM&)
		{
			std::cout<<"Guangzhou::on_exit\n";
		}
	};
	void on_south(const South&)
	{
		std::cout<<"China::on_south\n";
	}
	void on_north(const North&)
	{
		std::cout<<"China::on_north\n";
	}
	template<class Event, class MSM>
	void no_transition(const Event&,  MSM&, int state)
	{
		std::cout<<"no transition from state "
			<<state
			<<" on event "
			<<typeid(Event).name()<<"\n";
	}
	
	typedef Beijing initial_state;
	struct transition_table:mpl::vector<
		mfront::a_row2<Beijing,South,Shanghai,Beijing, &Beijing::on_south>,
		a_row<Shanghai,North,Beijing,&China::on_north>,
		a_row<Shanghai,South,Guangzhou,&China::on_south>,
		a_row<Guangzhou,North,Shanghai,&China::on_north>
	>{};
};
typedef mback::state_machine<China> CHINA;
int test_meta_main()
{
	CHINA c;
	c.start();
	c.process_event(South());
	c.process_event(South());
	c.process_event(South());//no trans
	c.process_event(North());
	c.process_event(North());
	c.process_event(North());
	c.process_event(South());
	c.process_event(South());
	c.process_event(North());
	c.process_event(North());//no trans
	return 0;
}