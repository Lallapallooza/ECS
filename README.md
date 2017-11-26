##  README OUTDATED BUT YOU CAN STILL USE IT FOR OVERVIEW


##  Simple ECS in C++
  This is an implement of ECS pattern in C++. It contains:
  
      -Entity
      -Component
      -System
      -Pool
      -Events
      -Matcher
      -Group
      -Reactive variables
      -Some helper tools

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
		static int id;
	  };
	  int ComponentPosition::id = 0;

 Every component must have id field and also must be initialized with zero. This allows Collector create unique ids for each component, it is needed for Matcher (create unique groups) and Entity (to make fast cast).
 For better navigation you may name your components, for example, like this: "Component_Name_".

##  Entity class
Class BaseEntity contains list of components and methods for adding, removing and getting your components.
    
    class EntityTree : public ECS::BaseEntity{};
    
  Here is an example how you can work with your entity:
	
       std::shared_ptr<EntityTree> tree = std::make_shared<EntityTree>();	//instantiating entity
       tree->addComponent(comp_position, comp_sprite);										//comp_position and comp_sprite are base of BaseComponent
       
All Entities must allocate on heap!

##  System class
  Every system (if it is not a helper system) must have at least two static methods: update and subscribe.
  Update method is basic method in our system. It allows to subcribe system to ECS::tools::event and call it using the  ECS::Update(). Method update incapsulating logic of a system, also can add some method to your system.
	
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
        
Helper system allows to make logic of programm more independent and easier to edit. For example, we shouldn't write all code about inventory i.e. trading, showing, correct checker or other, instead of this we create nested system (system without subscribe method).
example:

		class SystemA : ECS::BaseSystem
		{
			static void update()
			{
				auto attr = ECS::TPool<ComponentAttributeStrenght>::getComponents()[0];
				//
		 		// Some actions
		 		//
				SystemB::calculateDamage(attr); 
			}
		};

		class SystemB : ECS::BaseSystem
		{
			static int calculateDamage(ComponentAttributeStrenght &comp);
		};
    

In this example we create new system to divide logic of programm. If you need some function which is called in different system, create new system, if it calls only in one system, add method in it.


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
TPool has few methods for serialization, you can create serialization function for only one component and attach it into pool, and then method serialize(), desezalize() calls you serializator/deserializator, which is used for each component in TPool.
Also TPool gives you opportunity to add and remove component yourself.

##  Matcher class
That class allows to match entities with component you need
Example:
	
	ECS::Matcher::matchNew<Comp1, Comp2, Comp3, Comp4>();
	
You can list as much components as you need, because it works with "variadic" argument number.
Method MatchNew creates new Match, if entities with such components exist, it will return unordered_set 
of pointers on that entities else returns unordered_set with size 0
When you already make match, you can use match method and get the result of last match with same arguments.
Try to use match only once, because its expensive to collect it every update.

##  Group class
That class allows to group entities
Example: 

	ECS::Group<g_("Some")>::add(e1);
	ECS::Group<<g_("Some")>::add(e2);
	ECS::Group<<g_("Some")>::add(e3);
	
That class allows you to collect, add, remove entites from every place of programm. 

	g_("Some") 

it is a unique id, which you may choose as you wish.

