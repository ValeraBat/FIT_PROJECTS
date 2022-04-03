% Zadání č. 21:
% Napište program řešící úkol daný predikátem u21(LIN,VOUT), kde LIN je  
% vstupní celočíselný seznam obsahující čísla v intervalu <-10,10> a VOUT 
% je proměnná, ve které se vrací součin všech lichých čísel seznamu LIN. 

% Testovací predikáty:                         		% LOUT        
u21_1:- u21([5,3,2,4,-9,12,17],VOUT),write(VOUT).	% -2295
u21_2:- u21([8,10,2,4],VOUT),write(VOUT).		% 0
u21_3:- u21([5,3],VOUT),write(VOUT).			% 15
u21_r:- write('Zadej LIN: '),read(LIN),
        u21(LIN,LOUT),write(LOUT).

u21(LIN,VOUT):- oddNumbers(LIN,L2),list_mul(L2,VOUT).

oddNumbers([],[]).
oddNumbers([H|T],L1):-
    integer(H),
    (H mod 2 =:=1 -> L1=[H|T1],oddNumbers(T,T1);
    oddNumbers(T,L1) ).


list_mul([X],X):-!.
list_mul([H|T],P):-
    list_mul(T,P1),
    P is H*P1.
list_mul([],0).
