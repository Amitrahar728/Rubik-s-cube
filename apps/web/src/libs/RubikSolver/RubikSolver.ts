import { initialRubik } from '@/data';
import { orderedSides, type Sides, type VisibleSide } from '@/domain';
import { type Encoded } from '@/libs/encoder';
import Cube, { type CubeState } from 'cubejs';
import shuffle from 'lodash.shuffle';
import { indexedSides } from '../encoder/indexedSides';

export class RubikSolver {
  static solvedEncoded: string = new Cube().asString();
  static callback: ((solution: string | null, encoded: Encoded) => void) | null = null;
  static worker = {
    terminate() {
      console.log('[RubikSolver] Mock worker terminated.');
    }
  };

  static initSolver(cb: (solution: string | null, encoded: Encoded) => void) {
    this.callback = cb;
    fetch('/api/init')
      .then((res) => res.json())
      .then((data) => {
        console.log('[RubikSolver] Solver API initialized. Status:', data);
      })
      .catch((err) => {
        console.error('[RubikSolver] Failed to initialize solver API:', err);
      });
  }

  static shuffle() {
    const randomCube = Cube.random();

    const unorderedEncoded = shuffle(orderedSides).join('');

    const swapMap = orderedSides.reduce(
      (acc, curr, i) => ({ ...acc, [curr]: unorderedEncoded[i] }),
      {} as Record<VisibleSide, VisibleSide>
    );

    const encodedShuffle = (randomCube.asString().split('') as VisibleSide[])
      .map((side) => swapMap[side])
      .join('');

    const nameShuffleMap = indexedSides
      .map((v, i) => ({ [v]: `${encodedShuffle[i]}${v[1]}` }))
      .reduce((acc, curr) => ({ ...acc, ...curr }), {});

    const shuffledSides = initialRubik.map(({ sides: c }) =>
      c.map((m) => {
        if (m === '-') return '-';
        else return nameShuffleMap[m];
      })
    ) as Sides[];

    return shuffledSides;
  }

  static fromString(string: string) {
    return Cube.fromString(string).asString();
  }

  static toJson(string: string): CubeState {
    return Cube.fromString(string).toJSON();
  }

  static inverse(string: string) {
    return Cube.inverse(string);
  }

  static solve(encoded: Encoded) {
    fetch('/api/solve', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({ cube: encoded.cube })
    })
      .then((res) => {
        if (!res.ok) {
          throw new Error('Failed to solve cube via API');
        }
        return res.json();
      })
      .then((data) => {
        if (this.callback) {
          this.callback(data.result, encoded);
        }
      })
      .catch((err) => {
        console.error('[RubikSolver] Solve error:', err);
        if (this.callback) {
          this.callback(null, encoded);
        }
      });
  }

  static move(string: string, moves: string[]) {
    if (moves.length === 0) return string;
    const cube = Cube.fromString(string);
    cube.move(moves.join(' '));
    return cube.asString();
  }
}
