# blackjack-sdl
Blackjack in C using SDL3.

Uses 5-card Charlie rule (player's 5 cards beats dealer's 21; no cards are drawn on dealer blackjack so dealer's blackjack still beats player's 5 cards).

Dealer hits on soft 17 (e.g. ace+6). TODO.
```
mkdir build
cd build
cmake ..
make
./src/main
```