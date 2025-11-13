// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:18:26 - Wed

// DEMO.CPP

// This is an example class from Chapter 4 of the C++ Tutorial. This
//     class shows when constructors and destructors are called for
//     global, local, and static local objects.

#include <iostream.h>
#include <string.h>

class Demo
{
public:
    Demo( const char *nm );
    ~Demo();
private:
    char name[20];
};

Demo::Demo( const char *nm )
{
    strncpy( name, nm, 20 );
    name[19] = '\0';
    cout << "Constructor called for " << name << '\n';
}

Demo::~Demo()
{
    cout << "Destructor called for " << name << '\n';
}

void func()
{
    Demo localFuncObject( "localFuncObject" );
    static Demo staticObject( "staticObject" );

    cout << "Inside func" << endl;
}

Demo globalObject( "globalObject" );

void main()
{
    Demo localMainObject( "localMainObject" );

    cout << "In main, before calling func\n";
    func();
    cout << "In main, after calling func\n";
}
