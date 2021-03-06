Shared Explicit Instantiation
=============================

How to scale the reuse of a function *name* without worry of naming collisions?

This problem arose for me while trying to implement a theoretically complete version of the
list *map* operator (within the functional programming paradigm). My model has 9 configuration parameters based
on things like whether I want the map to iterate forward or backward over the list; what type of pointer I'm
working with (array, singly linked, doubly linked); whether I'm allocating memory or it is assumed to already
exist or if I want to deallocate the input list as I go; etc.  Each parameter has between 2-4 options,
and in total this has left me with several thousand variants of what is otherwise the same algorithmic pattern.

As you can see, there are situations where one has an inventory of functions which otherwise---intuitively---are
thought to have the same name. How do you mitigate that then? It's not at all practical to name each and every
function differently by hand or imagination when there are that many, it doesn't scale. So how then do we solve this problem?

My solution is to use template parameters to explicitly specialize each variant function comprising the inventory. Here's why:

### enumeration values as template parameters

Let's look at part of the [**template.h**](template.h) header file for starters:

> typedef unsigned long size\_type;
>
> template &lt; size\_type N1 , size\_type N2 , size\_type N3 , size\_type N4 , size\_type N5 &gt;
>
> struct godel { ... };

By declaring integer values (here "unsigned long") as the template parameter type, I can call the template by means
of named *enum* values thus making my inventory naming system much more human readable.

### declaring a templated structure instead of a templated function

You may have noticed we're defining functions, but instead of templating functions we are templating classes,
or equivalently *struct*s.  This is technically a hack, but I have a few good reasons for it.

First of all, I use the *struct* as a way of providing namespace scope. Thus you can declare the function within.
This is also why I declare the function as static, telling the compiler that each instance of the struct shares
the same function, and more importantly, it can now be called without actually instantiating a class.

The proper alternative is to use an actual *namespace* declaration, but structs provide the other benefit that they
themselves are recognized as types, which means they can be passed as template parameters to other templated objects.
This I have found to be quite useful as a general strategy for many other programming contexts.

More importantly, the nature of your functions when taking this approach will likely be that they have small variations
in their code body, but also in their code header as well. For example with my above *map* I had some versions
which took a "size\_type & count" variable as input while other variations of the map operator did not. At the same time,
some variations had the same input args, but varied internally within the algorithm itself. In such a case, those functions
having the same *signature* would not be distinguishable by the compiler which would complain. Taking a templated struct
approach solves this issue, though the solution as to why may not yet be fully clear.

The downside of declaring the main template as a struct is that we have to *declare* each function signature within this
template header.

> static size\_type encoding();
>
> static size\_type encoding(size\_type x);

Keep in mind we don't need to *define* a function for each signature, only the ones we know we will use,
though we'll get to that.

### preprocessor macros

Let's now look at the [**template.cpp**](template.cpp) file. You'll notice the use of a lot of macros.

> \#define declare5(N1, N2, N3, N4, N5)
>
> \#define declare4(N1, N2, N3, N4)

This isn't absolutely necessary for this idiomatic approach, but in a lot of ways it's preferred.
This exact example is artificial, if you look at the algorithm itself, it's not the most useful function,
but it's simple and it still demonstrates the point overall.

The main use of macros here is to quickly build a large repertoire of code, which I have done in this file. There are
5 macro arguments and each takes 5 values, so 5^5 = 3,125 distinct functions are produced. The *map* example mentioned
above had more.

### explicit specialization

If you've ever done template programming, you'll know there are *linking* subtleties that arise due to the special nature
of how templates actually work.

When writing non-template code, each module of code is actually split up into two parts: Header files (.h) where you
provide declarations, and source files (.cpp) where you provide actual definitions. Headers are like *tables of content*
while sources are like the actual *content*.

When compiling non-templated C++ code, to tell the compiler to compile a given .cpp file it looks at that file, and
it looks at the .h equivalent, and makes sure they're in agreement. The table of contents says certain structures and
functions with particular signatures should exist within the content itself, and so the content source file should match.
Aside from that, for each separate .cpp file the compiler turns them into what are called object files (machine code),
and then for each separate object file the compiler using what's called the *linker* combines them into one large file
which is your final output (more or less).

This approach to compiling is more complicated (than just plug-and-play compiling all at once), but it exists as an
engineering best practice: mitigating the complexing of compiling ever more complicated programs. By splitting up the
compilation, you've modularized it so if you need to change a small piece of code, you don't have to recompile everything,
you only need recompile that module. The reason you split up header and source files is that when one module uses the
code within another, it only needs to see its table of contents (which is smaller and easier to process). It uses that
to do some type checking, trying to catch bugs and all that.

This is good practice, but it wasn't designed with templates in mind---template came after.  If you did things in the
conventional way but with templates, you would split it up into its declaration and its definition the same way.
Unfortunately a template definition is not a true definition, it is kind of like a *promise* of a definition.
It says that as soon as an actual instance of the template (one with all its parameters clearly assigned) is called in
actual code, then it will create that true definition.

The subtle linking problem ends up being a problem of assumption. If for example you call the template in the
**main.cpp** file, it'll check the template table of contents **template.h** to see if the declaration is there.
Then, when compiling the **main.cpp** file into an object file, having checked the **template.h** file, it says it's satisfied
with the declaration and simply assumes a definition exists elsewhere. Then, when compiling the **template.cpp** file,
the compiler doesn't yet know a true definition is needed elsewhere, so it doesn't actually create one. So both
object files are created independently of each other, but when the linker goes to link them, it discovers there is
a table of contents reference to the needed template, but no actual code to go along with it. It's like having
a table of contents where one whole chapter is said to be there but actually isn't.

### header declaration vs. source definition

This template linking subtlety is a known problem, and those who maintain the language as their solution have added
to the standard grammar *explicit specialization* where you can *force* a version of an otherwise promised template definition.

If we look again at the **template.cpp** code, one such explicit specialization would look something like this:

> template &lt;&gt;
>
> size\_type godel&lt;1,1,1,1,2&gt;::encoding()
>
> {
>
>	return 25410;
>
> }

Again, having used macros, it looks a bit different in the file itself, and the fact that I streamlined the numeric computation
(which the compiler would resolve at compile-time anyway).

Otherwise, without explicit specialization, the typical solution to this problem is to both declare and define the
template in the same file---no separate header and source file, just a header file.  In such cases, I myself usually
define the content details inline (within the declaration itself).  This approach could have been taken here,
the only problem with that is the 3,000+ variants of template code the compiler would have to parse.
It's unlikely most of those template variants would even be used, no harm no foul, but the compiler still goes
through that code looking for syntax errors, and with such a large code base, the compiler tends to choke.
Not in the sense that it crashes, but my g++ compiler for example takes many seconds to run through all of that,
and if you're relying on this style of programming the wait time is only going to get worse. The *map* operator
I was building is actually part of a larger template library which adheres to this style. It's okay if you only
have to compile every once and a while, but if you're prototyping, feedback is a good thing and having to wait
all that extra time every time you make a small change in your code is unproductive and demoralizing.

This is why we again split it up into separate header and source files. Explicit Specialization allows us to do that.
By declaring our template structure and declaring its member functions in the **template.h** header file, we have provided
an acceptable table of contents telling any files which use this module what it can expect.

We then define only those explicit specialization instances we know we will use in advance in the source file.
In our case, given the nature of this construct (how we're using it to scale namespacing), our parameter list
doesn't vary in its type, and we're interested in every variant in a particular range, so it's simple.

So with this we can return to the modularization approach in compiling: We now compile this **template.cpp**
file as an object file. It still takes several seconds (would be minutes for a larger inventory),
but you only need to do it once. Then for example, looking at the [**main.cpp**](main.cpp) file we include
the **template.h** table of contents (which the compiler parses in no time at all unlike
its **template.cpp** counterpart), and thus we can make a call to any given function within our inventory,
using the same name for all functions, not having to worry about name collisions.

Good stuff.

