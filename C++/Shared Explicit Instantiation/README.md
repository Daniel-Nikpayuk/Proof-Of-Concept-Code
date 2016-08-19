Shared Explicit Instantiation
=============================

How to scale the reuse of a function *name* without worry of naming collisions?

This problem arose for me while trying to implement a theoretically complete version of the
list *map* operator (within the functional paradigm). My model has 9 configuration parameters based on things
like whether I want the map to iterate forward or backward over the list; what type of pointer I'm working with
(array, singly linked, doubly linked); whether I'm allocating memory or it is assumed to already exist or if
I want to deallocate the input list as I go; etc.  Each parameter has between 2-4 options, and in total this
has left me with several thousand variants of what is otherwise the same algorithmic pattern.

As you can see, there are situations where one has an inventory of functions which otherwise---intuitively---are
thought to have the same name. How do you mitigate that then? It's not at all practical to name each and every
function differently by hand or imagination when there are that many, it doesn't scale. So how then do we solve this problem?

My solution is to use template parameters to explicitly specialize each variant function comprising the inventory. Here's why:

Enumeration values as Template parameters
-----------------------------------------

Let's look at the [./template.h](template.h) header file for starters:

> typedef unsigned size\_type;
>
> template<size\_type N0, size\_type N1, size\_type N2, size\_type N3, size\_type N4, size\_type N5>

By declaring integer values (here "unsigned") as the template parameter type, I can call the template by means
of named *enum* values thus making my inventory naming system more human readable.

Declaring a templated structure instead of a templated function
---------------------------------------------------------------

This is technically a hack, but I have a few good reasons for it.

First of all, I use the *struct* as a way of providing namespace scope. Thus you can declare the function within.
The proper alternative is to use an actual *namespace* declaration, but structs provide the other benefit that they
themselves are recognized as types, which means they can be passed as template parameters to other templated objects.
This I have found to be quite useful.

More importantly, the nature of your functions when taking this approach will likely have small variations in their
argument list as well. For example with my above *map* I had some versions which took a "size\_type & count" variable
as input while other variations of the map operator did not. At the same time, some variations had the same input args,
but varied internally within the algorithm itself. In such a case, those functions having the same *signature* would not
be distinguishable by the compiler which would complain. Taking a templated struct approach solves this issue, though
the solution as to why may not yet be clear.

The downside of declaring the main template as a struct is that we have to *declare* each function signature within this
template header. Keep in mind we don't need to *define* a function for each signature, only the ones we know we will use,
though we'll get to that.

Preprocessing Macros
--------------------

Let's now look at the **template.cpp** file. You'll notice the use of a lot of macros.

This isn't absolutely necessary for this idiomatic approach, but in a lot of ways it's preferred.
This exact example is artificial, if you look at the algorithm itself, it's not the most useful function,
but it's simple and it still demonstrates the point overall.

The main use of macros is to quickly build a large repertoire of code, which I have done in this file. There are 6
macro arguments and each takes 6 values, so 6^6 = 46,656 distinct functions are produced. Actually that's not true
given the exact nature of the function defined, but let's pretend otherwise. At the very least there are a lot distinct
functions.

Explicit Specialization
-----------------------

If you've ever done template programming, you'll know there are *linking* subtleties that arise due to the special nature
of how templates actually work.

When writing non-template code, each module of code is actually split up into two parts: Header files (.h) where you
provide declarations, and source files (.cpp) where you provide actual definitions. Headers are like *tables of content*
while sources are like the actual *content*.

When compiling non-templated C++ code, to tell the compiler to compile a given .cpp file. It looks at that file, and
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
conventional way then you would split up a template into its declaration and its definition the way you would with regular
C++ code. Unfortunately a template definition is not a true definition, it is kind of like a *promise* of a definition.
It says that as soon as an actual instance of the template (one with all its parameters clearly assigned) is called in
actual code, then it will create that true definition.

The subtle linking problem ends up being a problem of assumption. If for example you call the template in the
**main.cpp** file, it'll check the template table of contents (template.h) to see if the declaration is there.
Then, when compiling the main.cpp file into an object file, having checked the template.h file, it says it's satisfied
with the declaration and simply assumes a definition exists elsewhere. Then, when compiling the template.cpp file,
the compiler doesn't yet know a true definition is needed elsewhere, so it doesn't actually create one. So both
object files are created independently of each other, but when the linker goes to link them, it discovers there is
a table of contents reference to the needed template, but no actual code to go along with it. It's like having
a table of contents where one whole chapter is said to be there but isn't actually there.

Header Declaration vs. Source Definition
----------------------------------------

This is a known problem, so one solution added to the language standard was to allow for *explicit specialization* where
you can *force* a version of an otherwise promised template definition.

If we look again at the **template.cpp** code, it would look something like this:

> template<>
> size\_type sum<0,1,2,3,4,5>::function(size\_type x)
> {
>	return (0+1+2+3+4+5)\*x;
> }

Again, having used macros, it looks a bit different, but only as a more automated approach at explicitly specializing
so many variants.

Otherwise, the typical solution to this problem is to both declare and define the template in the same file
(no separate header and source file, just a header file).

I myself usually define the content details inline (within the declaration itself; which additionally modifies compiler
optimizations). This approach could have been taken here, the only problem with that is the roughly 46,000 variants of template
code the compiler would have to parse. It's unlikely most of those template variants would even be used, no harm no foul,
but the compiler still goes through that code looking for syntax errors, and with such a large code base, the compiler
tends to choke. My g++ compiler takes a few minutes to run through all of that, and that is just not practical to do
every single time you want to make a small change.

This is why we again split it up into separate header and source files. Explicit Specialization allows us to do that.
By declaring our template structure and declaring its member functions in the template.h header file, we have provided
an acceptable table of contents telling any files which use this module what it can expect.

We then define only those explicit specialization instances we know we will use in advance in the source file.
In our case, given the nature of this construct (how we're using it to scale namespacing), we're interested in
every variant, so it's simple.

So with this we can return to the object modularization approach in compiling: We now compile this template.cpp file
as an object file. It still takes minutes, but you only need to do it once. Then for example, looking at the **main.cpp**
file we include the template.h table of contents (which the compiler parses with ease unlike its template.cpp counterpart),
and thus we can declare a call to any given function with our inventory, one which compiles in no time.

