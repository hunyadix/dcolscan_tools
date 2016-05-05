include Makefile.arch

MAIN_S = ./Resources/Sources/Programs/main.$(SrcSuf)
MAIN_O = ./Resources/Objects/main.$(ObjSuf)
MAIN_A = Dcolscan_creation$(ExeSuf)
OBJS          += $(MAIN_O)
PROGRAMS      += $(MAIN_A)

FITTER_S = ./Resources/Sources/Programs/my_fitter.$(SrcSuf)
FITTER_O = ./Resources/Objects/my_fitter.$(ObjSuf)
OBJS          += $(FITTER_O)

CONSOLE_COL_S = ./Resources/Sources/Includes/Console_colors/Console_colors.$(SrcSuf)
CONSOLE_COL_O = ./Resources/Objects/Console_colors.$(ObjSuf)
OBJS          += $(CONSOLE_COL_O)

CONSOLE_ACTOR_S = ./Resources/Sources/Includes/Console_actor/Console_actor.$(SrcSuf)
CONSOLE_ACTOR_O = ./Resources/Objects/Console_actor.$(ObjSuf)
OBJS          += $(CONSOLE_ACTOR_O)

COMMON_ACTORS_S = ./Resources/Sources/Includes/Console_actor/common_actors.$(SrcSuf)
COMMON_ACTORS_O = ./Resources/Objects/common_actors.$(ObjSuf)
OBJS          += $(COMMON_ACTORS_O)

TIMER_S = ./Resources/Sources/Includes/Timer/Timer.$(SrcSuf)
TIMER_O = ./Resources/Objects/Timer.$(ObjSuf)
OBJS          += $(TIMER_O)

TIMER_COL_S = ./Resources/Sources/Includes/Timer/Timer_colored.$(SrcSuf)
TIMER_COL_O = ./Resources/Objects/Timer_colored.$(ObjSuf)
OBJS          += $(TIMER_COL_O)

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:                  $(PROGRAMS)

$(MAIN_A): $(MAIN_O) $(FITTER_O) $(CONSOLE_COL_O) $(CONSOLE_ACTOR_O) $(COMMON_ACTORS_O) $(TIMER_O) $(TIMER_COL_O) 
	@printf "Compiling done, linking...\n"
	@$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
	$(MT_EXE)
	@echo "Succesful make..."
	@echo "...$@ is ready to use."

$(MAIN_O): $(MAIN_S)
	@printf "Compiling main...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Main compiled.\n\n"

$(FITTER_O): $(FITTER_S)
	@printf "Compiling fitter...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Fitter compiled.\n\n"

$(CONSOLE_COL_O): $(CONSOLE_COL_S)
	@printf "Compiling console colors...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Console colors compiled.\n\n"

$(CONSOLE_ACTOR_O): $(CONSOLE_ACTOR_S)
	@printf "Compiling console actor...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Console actor compiled.\n\n"

$(COMMON_ACTORS_O): $(COMMON_ACTORS_S)
	@printf "Compiling common actors...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Common actors compiled.\n\n"

$(TIMER_O): $(TIMER_S)
	@printf "Compiling timer...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Timer compiled.\n\n"

$(TIMER_COL_O): $(TIMER_COL_S)
	@printf "Compiling colored timer...\n"
	@$(CXX) $(CXXFLAGS) $(LIBS) -c $< $(OutPutOpt)$@
	@printf "Colored timer compiled.\n\n"

clean:
	@rm -f $(OBJS) core

distclean: clean
	@rm -f $(PROGRAMS) *Dict.* *.def *.exp \
	*.root *.ps *.so *.lib *.dll *.d *.log .def so_locations
	@rm -rf cxx_repository

.SUFFIXES: .$(SrcSuf)


#.$(SrcSuf).$(ObjSuf):
#	$(CXX) $(CXXFLAGS) -c $<

