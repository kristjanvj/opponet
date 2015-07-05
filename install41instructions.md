# Installation instructions for the 4.1 port of Opposim for OMNeT++ 4.1 #

The port is available as a tarball under downloads -- it is not a part of the sourcetree at this time.

  * Create a new OMNeT++ project in the project explorer
  * You have to build and install the mobility framework (MF) for OMNeT 4 and import as a project into the omnet workspace
  * Extract the opposim port and import as a project into the workspace.
  * You have to add the dependencies for MF to the opposim project: properties | c/c++ general | paths and symbols -- check rtn in references (for some reason MF is imported as rtn, may be different for you).
  * Opposim uses libxml2. There is excellent xml support in omnet but we used libxml for some obscure compatibility issues w. other projects. You may have to install libxml2 on your system. Then you have to add the include path to includes for the opposim project -- mine is at /usr/include/libxml2.
  * Finally, modify BasicModule.cc from MF as follows:

```
cModule *BasicModule::findHost(void) const 
{
    cModule *mod = getParentModule();  // TODO: CHECK KVJ

    /*
    for (mod = getParentModule(); mod != 0; mod = mod->getParentModule())
    {
        if (strstr(mod->getName(), "host") != NULL || strstr(mod->getName(), "Host") != NULL)
            break;
    }
    */
    if (!mod)
        error("findHost: no host module found!");

    return mod;
}
```

I wanted complete flexibility in naming simulation entities, not the hostxx convention of MF. Opposim assumes parent is strictly the hierarchial parent.