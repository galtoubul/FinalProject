<h1 class="code-line" data-line-start=0 data-line-end=1 ><a id="Interactive_Sudoku_0"></a>Interactive Sudoku</h1>
<p class="has-line-data" data-line-start="1" data-line-end="2"><strong><em>by Gal Toubul and Omri Berkovitch</em></strong></p>
<h2 class="code-line" data-line-start=2 data-line-end=3 ><a id="functionalities_2"></a>functionalities</h2>
<ul>
<li class="has-line-data" data-line-start="3" data-line-end="4">Solving Sudoku puzzles</li>
<li class="has-line-data" data-line-start="4" data-line-end="5">Editing existing and creating new Sudoku puzzles</li>
<li class="has-line-data" data-line-start="5" data-line-end="6">Supporting puzzles of any size</li>
<li class="has-line-data" data-line-start="6" data-line-end="7">Validating the board</li>
<li class="has-line-data" data-line-start="7" data-line-end="8">Saving and loading puzzles from files</li>
<li class="has-line-data" data-line-start="8" data-line-end="9">Randomly generating new puzzles</li>
<li class="has-line-data" data-line-start="9" data-line-end="10">Finding the number of solutions for a given board</li>
<li class="has-line-data" data-line-start="10" data-line-end="12">Auto-filling option for obvious cells (where only a single value is valid)</li>
</ul>
<p class="has-line-data" data-line-start="12" data-line-end="16">The program will have three main modes:<strong> Init, Edit and Solve</strong>.<br>
In the Solve mode, the user attempts to solve a pre-existing puzzle.<br>
In the Edit mode, the user edits existing puzzles or creates new ones, and saves these for opening in the Solve mode later. In the Init mode, the user loads a file to enter either Edit or Solve mode.<br>
The program starts in Init mode.</p>
<h2 class="code-line" data-line-start=17 data-line-end=18 ><a id="Main_Parts_17"></a>Main Parts</h2>
<ol>
<li class="has-line-data" data-line-start="18" data-line-end="19">Sudoku game logic</li>
<li class="has-line-data" data-line-start="19" data-line-end="20">Console user interface</li>
<li class="has-line-data" data-line-start="20" data-line-end="21">ILP solver algorithm (using gurobi framework)</li>
<li class="has-line-data" data-line-start="21" data-line-end="22">Random puzzle generator</li>
<li class="has-line-data" data-line-start="22" data-line-end="23">Exhaustive backtracking solver (for determining the number of different solutions)</li>
</ol>
