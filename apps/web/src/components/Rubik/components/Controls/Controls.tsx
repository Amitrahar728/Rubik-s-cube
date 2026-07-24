import { type Move, type Side } from '@/domain';
import { ChevronRightIcon, CircleIcon } from '@/icons';
import { Button } from '@/ui';
import { useColoring } from '@/hooks';
import clsx from 'clsx';
import type { CSSProperties } from 'react';
import classes from './Controls.module.css';

interface Props {
  move: (m: Move[]) => void;
  isMoving: boolean;
  isSolving: boolean;
  solution?: string | null;
  solutionIndex: number | null;
  gotoSolutionMove: (index: number) => void;
}

const moves: Move[] = [
  'U',
  'D',
  'R',
  'L',
  'F',
  'B',
  "U'",
  "D'",
  "R'",
  "L'",
  "F'",
  "B'",
] as const;

export function Controls({
  move,
  isMoving,
  isSolving,
  solution,
  solutionIndex,
  gotoSolutionMove,
}: Props) {
  const { sideToColorMapRef } = useColoring();
  const sideToColorMap = sideToColorMapRef.current;

  return (
    <div className={classes.container}>
      <div className={classes.controls}>
        {moves.map((moveName) => {
          const side = moveName[0] as Side;
          const color = sideToColorMap[side];
          const isPrime = moveName.endsWith("'");

          return (
            <Button
              square
              disabled={isSolving}
              key={moveName}
              onClick={() => {
                move([moveName]);
              }}
              className={clsx(classes.controlBtn, {
                [classes.prime]: isPrime,
              })}
              style={{
                '--btn-color': color,
              } as CSSProperties}
            >
              {moveName}
            </Button>
          );
        })}
      </div>

      <div className={classes.solutionViewer}>
        {solution ? (
          <>
            {solution.split(' ').map((move, index) => (
              <button
                onClick={() => {
                  if (!isMoving && !isSolving) gotoSolutionMove(index);
                }}
                className={clsx(classes.solutionMove, {
                  [classes.active]: solutionIndex === index,
                })}
                disabled={isMoving || isSolving}
                key={index}
              >
                {index === 0 ? null : <ChevronRightIcon color="inherit" />}
                <div className={classes.solutionMoveKey}>
                  {move === '-' ? <CircleIcon /> : move}
                </div>
              </button>
            ))}
          </>
        ) : null}
      </div>
    </div>
  );
}
