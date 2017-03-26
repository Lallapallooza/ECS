##  Simple ECS in C++
  This is an implement of ECS pattern in C++. It contains:
  
      -Entity
      -Component
      -System
      -Pool
      -Events

  Every ECS sytem contains three basic elements: components, entity and system.
  Pool and Events were added to make proccess of develepment easier.
	
##  Basics of ECS
 Component contains raw data, entity comprises container of components and system performs global actions on entities and components. 
	Pool allows your system to gain access to **registered** components by their type for O(1) complexcity. Pool were designed using templates. 
	If you want to know about events you can find information here [c#](https://msdn.microsoft.com/en-us/library/awbftdfh.aspx)). 
	You can instantiate or destroy components using collector.
	For additional information about ECS please follow this link [here](https://en.wikipedia.org/wiki/Entity–component–system).
	
##  Component class
Class BaseComponent contains only pointer to an entity (owner) of this component.

    class ComponentPosition : public ECS::BaseComponent
	  {
	  public:
		float x = 10;
		float y = 15;
	  };
	
 For better navigation you may name your components, for example, like this: "Component_Name_".

##  Entity class
Class BaseEntity contains list of components and methods for adding, removing and getting your components.
    
    class EntityTree : public ECS::BaseEntity
	  {
	  public:
	  	void set(float x, float y, sprite &sp)
	  	{
	  		std::for_each(begin(this->components), end((this->components)),
	  			[&](std::shared_ptr<ECS::BaseComponent>& comp)
	  		{
	  			auto pos_it = std::dynamic_pointer_cast<ComponentPosition>(comp); //cast each component to necessary type, dynamic cast will 					 																																		//return null if something is wrong with component			
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
    
  Here is an example how you can work with your entity:
	
       std::shared_ptr<EntityTree> tree = std::make_shared<EntityTree>();	//instantiating entity
       tree->addComponent(comp_position, comp_sprite);										//comp_position and comp_sprite are base of BaseComponent
       tree->set(10, 10, some_image);																			//set data
   
	Set method allows you to set data easier.

##  System class
  Every system must have at least two static methods: update and subscribe.
  Update method is basic method in our system. It gives opportunity to subcribe system to ECS::tools::event and call it using the     	   ECS::Update(). Method update should incapsulate logic of a system, also can to add some method to your system.
	
	That system prints the position ComponentPosition:
	
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
    
Collecting it:
  
  	auto comps = ECS::TPool<ComponentPosition>::getComponents();
  
Events are declared in "Tools.h" file. ECS::Update has 3 standart events:

    1.preUpdate
    2.Update
    3.lateUpdate
  
"Subscribe" on events allows to use in the game loop 3 lines of code:
    
    while(true)
    {
        ECS::preUpdate();
        ECS::Update();
        ECS::lateUpdate();
    }
    
##  Event class
Here is my implement of events in c++. It contains methods for adding and removing "subscribers" and also method to compare std::function. However, it doesn't work with lambdas.
  
    void func();
    void func2();
  
    ECS::tools::event my_event;
    my_event += func; 					//subscribe func on my_event
    my_event += func2; 					//subscribe func2 on my_event
    
    my_event(); 								//call func and func2
    
    my_event -= func; 					//erase func
    
    my_event(); 								//call only func2

##  Collector class
Collector class allows you to easily instantiate components and destoy them.

    auto my_comp = ECS::tools::Collector::instantiate<ComponentPosition>(); 				//instantiate component of ComponentPosition type, 		 																																									//create pool for it, and add in existing pool
		...																																							//some code
    ECS::tools::Collector::destroy(my_comp); //delete refs from TPool and Entity and my_comp	//destroying my_comp

##  PoolManager class

	auto all_entities = ECS::PoolManager::getEntities<ComponentPosition>() //return all entities with ComponentPosition

##  TPool class
That class allows to get component by their type for O(1) complexity:
  
    auto comps = ECS::TPool<ComponentBlablabla>::getComponents(); //return ref on list with all ComponentBlablabla
  
If you forget to register component, method "getComponents" won't return it.
Also TPool gives you opportunity to add and remove component yourself.

