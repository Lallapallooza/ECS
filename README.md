## Simple and powerful ECS on C++
  This is an implement of ECS pattern on C++ that includes such things as: 
  -Entity
  -Component
  -System
  -Pool
  -Events
  First 3 it's foundation of every ECS systems. Pool and Events my features to make code easy. Below I describe how to use and editing it
## Basics of ECS
  About what ECS and his foundation means you can read [here](https://en.wikipedia.org/wiki/Entity–component–system)
briefly components its "nude" data, entity its list of components and system its logic of out soft. Pool allows your system to collect all **registered** components by their type for O(1) complexcity. That trick able to exist by using templates how its works, I talk later. And some help tools such events (like in [c#](https://msdn.microsoft.com/en-us/library/awbftdfh.aspx)), Collector class (allows us to easy instantiate component and destroy them) and other one.

## Component class
  Class BaseComponent contain only pointer to entity(owner) of this component.
  Example how to create your own
    
	class ComponentPosition : public ECS::BaseComponent
	{
	public:
		float x = 10;
		float y = 15;
	};
  Tip: call your components like Component_Name_. That allow you to easy navigate in your code. Same tip works for systems and entities
## Entity class
  Class BaseEntity contain list of components and methods to add, remove and get components.
  Example how to create your own and use it
    
    class EntityTree : public ECS::BaseEntity
	{
	public:
		void set(float x, float y, sprite &sp)
		{
			std::for_each(begin(this->components), end((this->components)),
				[&](std::shared_ptr<ECS::BaseComponent>& comp)
			{
				auto pos_it = std::dynamic_pointer_cast<ComponentPosition>(comp); //cast each component to type we need, because we using dynamic cast if return null if component "wrong"
				if (pos_it)                                                       
				{																                                  
					pos_it->x = x;                                                  
					pos_it->y = y;
				}
                auto sp_it = std::dynamic_pointer_cast<ComponentSprite>(comp);
                if (sp_it)
                {
                	sp_it->sp = sp;
                }
			});
		}
	};
    
    
    //now you able to use it
    std::shared_ptr<EntityTree> tree = std::make_shared<EntityTree>(); //instantiate our entity
    tree->addComponent(comp_position, comp_sprite); //comp_position and comp_sprite are base of BaseComponent
    tree->set(10, 10, some_image); //set data
   
   If you want you may not create set method or create it by your own way
   Tip: creating set method allow you a easier set data as for me
   

## System class
  Every systems should have at least two static methods.
  1.update
  2.subcribe
  update method for we get opportunity to "subscribe" it yo out event and subcribe for subcribe :)
  Example how to create your own
	
    class SystemPrinter : public ECS::BaseSystem
	{	
	public:
		static void subscribe()
		{
			ECS::Update += update;
		}

		static void update()
		{
			auto comps = ECS::TPool<ComponentPosition>::getComponents(); //get all component of type
			if (comps.size() != 0)
			{
				for (const auto &x : comps)
				{
					std::cout « x->x « ' ' « x->y « std::endl;
				}
			}
		}
	};
    
  That system just print position of all position components. The way how we collect it 
  
    auto comps = ECS::TPool<ComponentPosition>::getComponents();
  
  how it works read below. ECS::Update its one of 3 standart events in our ECS
    
    1.preUpdate
    2.Update
    3.lateUpdate
  
  they all declared in "Tools.h" file. "Subscribe" on events allow us to use in game loop just write 3 line of code
    
    while(true)
    {
        ECS::preUpdate();
        ECS::Update();
        ECS::lateUpdate();
    }
    
## Event class
  Just my implement of events on c++. Its contains methods for add and remove "subscribers" and method to compare std::function. **Be careful, aint works with lambdas!**
  Example how to use
  
    void func();
    void func2();
  
    ECS::tools::event my_event;
    my_event += func; //subscrive func on my_event
    my_event += func2; //subscrive func2 on my_event
    
    my_event(); //call func and func2
    
    my_event -= func; //erase func
    
    my_event(); //call only func2

## Collector class
  Collector class allows you easy to instantiate components and destoy them. That guy for you register/unregister it in PoolManager and destroy arr refs in TPool and Entity.
  Example:

    auto my_comp = ECS::tools::Collector::instantiate<ComponentPosition>(); //instantiate component of ComponentPosition type and create pool for it or add in exist pool
    //some code
    ECS::tools::Collector::destroy(my_comp); //delete refs from TPool and Entity and my_comp

## PoolManager class
  PoolManager just a class to help Collector to register and destoy component, you able to use that methods too, but better use Collector for instantiate and destoy. Also its get method getEntities, that method return list of all entities who own component of some type
    auto all_entities = ECS::PoolManager::getEnteties<ComponentPosition>() //return all entities with ComponentPosition

## TPool class
  That class allows us to get component by their type for O(1) complexity. In description of systems you saw how we use it
  
    auto comps = ECS::TPool<ComponentBlablabla>::getComponents(); //return ref on list with all ComponentBlablabla
  
  Remember, if you forgot to register component method getComponents aint return that one
  Also TPool gives you opportunity to add and remove component by your hands

