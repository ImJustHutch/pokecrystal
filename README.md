# Pokémon Shining Crystal [![Build Status][ci-badge]][ci]

This is a hack of Pokémon Crystal.

## Design Philosophy

My approach with hacks is to remove unnecessary tedium and to give the player as much choice & freedom as possible. I believe that a little quality of life goes a long way, and that a handful of features from modern games can breathe a lot of life back into the older ones. The original Pokémon Crystal has a lot to offer, and I wanted it to shine through, hence the name. 

That being said, GSC definitely still has its flaws. This hack attempts to mitigate those flaws or fix them altogether.

### Pokémon

I have a hard rule to keep things as legal as possible when it comes to the Pokémon themselves for the purposes of transferring up. That means no changing the levels or locations of wild Pokémon, no changing the move sets<sup>*</sup>, and no changing shiny odds. There are some workarounds for this such as in-game trades & eggs, so I use those to give the player Pokémon that are otherwise unobtainable in the vanilla game. The only Pokémon you won't be able to get are the Legendary Birds, Mewtwo, and Mew since I could not come up with a way to organically add them to the game without making them illegal. Fortunately, they can be acquired in any Gen 1 game.

<sup>*</sup> Many Pokémon have moves within their level-up set that are also TMs, which can be reordered without breaking legality. I am considering playing around with move sets in this way, but may decide it's not worth implementing due to the inconsistent nature of it.

### Trainers, Experience, & Difficulty (or the lack thereof)

While I have reservations about modifying wild Pokémon for legality reasons, there's no reason why the underleveled trainer Pokémon ought to stay the same. As you probably know, GSC suffers from a very flat level curve once you get past Sudowoodo. Trainers from Ecruteak to Cianwood and even up to the Lake of Rage all have Pokémon in the mid-high teens, with a scarce few in the low twenties. The intention is to give the player the freedom to take whichever route they want, but you end up plowing through weak trainer after weak trainer. This has a side effect of not allowing the player's team to gradually level up as they progress, leading to a tedious, unrewarding grind through the mid game. Not to mention, most trainers only have a couple Pokémon compared to your (likely) full team of 6, including Gym Leaders!

Because of this, I plan to increase the level of trainers past Ecruteak in either direction by about 5 levels across the board with Gym Leaders receiving some fine tuning based on how much experience that will yield the average team. I also plan to make the 5th, 6th, and 7th Gym Leaders' teams a bit tougher by adding or replacing members of their party. This may even extend to other bosses like the Rocket Admins, Rival fights, Clair, Elite 4, & Champion, but we'll see. Not only will this give the player's party more experience, but it should also adequately challenge the player more as they progress through the game.

### Battle

In Gens 1-3, odd numbered badges boost a certain stat by 12.5% or an increase of 1/8 (with the Lightning Badge increasing Speed a 2nd time in GSC), but a lesson known mechanic unique to Gen 2 are the type boosts. Each badge boosts its corresponding type by 12.5% as well (e.g. Zephyr Badge boosts Flying, Boulder Badge boosts Rock, etc.). This effect is a bit flawed, however, since it means that some moves/Pokémon that share types with a badge you've received will get power boosts while other moves/Pokémon don't and won't get an equivalent boost until much later in the game. Also, there are 17 types and only 16 badges...which means there's an odd one out: Dark. It's possible to add a Dark boost for, say, clearing the Radio Tower (since the top Rocket Admin mainly uses Dark Pokémon), but you'd still be left with the "asymmetrical" boosts.

It made the most sense to me to remove the type boosts altogether, but why stop there? Going a step further, I've also removed the stat boosts as well to keep things more in line with games post Gen 3, where badges don't give any kind of boost. This will help make the game feel a lot more consistent instead of favoring one kind of attacker or certain types feeling stronger than others. Not only that, but the additional 25% chance for the AI to miss status moves has also been removed. These changes level the playing field naturally and make the game more challenging without making it frustrating.

### Colors

This hack uses adjusted colors meant to look like how it would on a GBC and is intended to be viewed on a backlit screen (e.g. GBA SP, modded GBA/GBC, mGBA, etc.). If you want to undo this, copy the gfx/pokemon & gfx/trainers folders from pret/pokecrystal into the repository as well as macros/gfx.asm.

### and more!

Read the changelog(CHANGELOG.txt) for a detailed list of changes.

You can reach me via Discord @ImJustHutch

pret/pokecrystal stuff:
To set up the repository, see [INSTALL.md](INSTALL.md).

## Credits

Credit to rangi42 and Grate Oracle Lewot for answering a ton of my questions in the pret Discord.

Credit to Raja239 for the Move Maniac script & scrolling menu.
https://github.com/RaJah239/Johto-Frontier/commit/8d95dabab391e6477e8df95a61d4bcf4126d6bdc

A lot of the changes made can be found on the pokecrystal wiki in the Tutorial section, so credit to the authors of the following tutorials:

* [**Make HM Moves Forgettable**](https://github.com/pret/pokecrystal/wiki/Tips-and-tricks#make-hm-moves-forgettable)
* [**Simplify the Clock Reset Procedure**](https://github.com/pret/pokecrystal/wiki/Simplify-the-Clock-Reset-Procedure)
* [**Add a New Scene Script**](https://github.com/pret/pokecrystal/wiki/Add-a-new-scene-script)
* [**Move Tutor and Tutor Moves**](https://github.com/pret/pokecrystal/wiki/Move-Tutor-and-Tutor-Moves)
* [**Add a Move Relearner**](https://github.com/pret/pokecrystal/wiki/Add-a-Move-Relearner)
* [**Improve the Outdoor Sprite System**](https://github.com/pret/pokecrystal/wiki/Improve-the-outdoor-sprite-system)
* [**Short Beeping Noise for Low HP**](https://github.com/pret/pokecrystal/wiki/Short-beeping-noise-for-low-HP)
* [**Remove the Artificial Save Delay**](https://github.com/pret/pokecrystal/wiki/Remove-the-artificial-save-delay)
* [**Make the Lottery Corner Generate a Lucky Nmber Daily instead of Weekly**](https://github.com/pret/pokecrystal/wiki/Make-the-Lottery-Corner-generate-a-lucky-number-daily-instead-of-weekly)
* [**Make New Battle Text to Distinguish Status Move Misses and Fails**](https://github.com/pret/pokecrystal/wiki/Make-new-battle-text-to-distinguish-status-move-misses-and-fails)
* [**Kurt Makes Pokéballs Instantly**](https://github.com/pret/pokecrystal/wiki/Kurt-Makes-Pokeballs-Instantly)
* [**Harvest Multiple Items from Fruit Trees**](https://github.com/pret/pokecrystal/wiki/Harvest-multiple-items-from-fruit-trees)
* [**Prevent Steel Types from being Poisoned by Twineedle**](https://github.com/pret/pokecrystal/wiki/Prevent-Steel%E2%80%90types-from-being-poisoned-by-Twineedle)
* [**Remove the 25% Failure Chance for AI Status Moves**](https://github.com/pret/pokecrystal/wiki/Remove-the-25%25-failure-chance-for-AI-status-moves)
* [**Remove the Gym Badge Boosts**](https://github.com/pret/pokecrystal/wiki/Remove-the-gym-badges-boosts)
* [**Infinitely Reusable TMs**](https://github.com/pret/pokecrystal/wiki/Infinitely-reusable-TMs)
* [**Automatically Reuse Repel**](https://github.com/pret/pokecrystal/wiki/Automatically-reuse-Repel)
* [**Adding a Tradeback NPC**](https://github.com/pret/pokecrystal/wiki/adding-a-tradeback-npc)
* [**Allow Using a Field Move If the Pokémon Can Learn It**](https://github.com/pret/pokecrystal/wiki/Allow-Using-a-Field-Move-if-the-Pokemon-Can-Learn-It)

And a final thanks to the community as a whole for having all these resources available so that making your own hack is as easy as it has ever been!

## See also

- [**FAQ**](FAQ.md)
- [**Documentation**][docs]
- [**Wiki**][wiki] (includes [tutorials][tutorials])
- [**Symbols**][symbols]
- [**Tools**][tools]

You can find us on [Discord (pret, #pokecrystal)](https://discord.gg/d5dubZ3).

For other pret projects, see [pret.github.io](https://pret.github.io/).

[docs]: https://pret.github.io/pokecrystal/
[wiki]: https://github.com/pret/pokecrystal/wiki
[tutorials]: https://github.com/pret/pokecrystal/wiki/Tutorials
[symbols]: https://github.com/pret/pokecrystal/tree/symbols
[tools]: https://github.com/pret/gb-asm-tools
[ci]: https://github.com/pret/pokecrystal/actions
[ci-badge]: https://github.com/pret/pokecrystal/actions/workflows/main.yml/badge.svg
