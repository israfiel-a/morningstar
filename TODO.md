Current Objectives / Fixes:
- EGL - We need an OpenGL/EGL context to be creatable/usable within the application. Rendering each individual pixel by hand is not going to cut it.
- Include Guards - Many of the include guards make little to no sense. Come up with a naming convention that fixes this issue.
- WARNINGNO - A sort of in-application ERRNO for any warnings that may be given by functions. Would be very, very useful in detecting errors before they happen.
- Error Extension - A string value that can be sent to the error reporter to add more information to an error report. This is useful for ERRNO-sending functions.

Goal Objectives / Fixes:
- "Stale-free mode" for the case of the application running out of memory and general memory safety. This would add significantly more functionality to the `wrapped_ptr` object, as it would put a timestamp on the pointer which would be checked within a separate thread to make sure the object is still in use.
    - Idea source: [here](https://codereview.stackexchange.com/questions/293244/safer-easier-pointer-allocation-interface-in-c-iso-c11/293246?noredirect=1#comment584195_293246)