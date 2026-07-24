import { config } from '@/config';
import { CloseIcon, CubeIcon, GitHubIcon } from '@/icons';
import { Button } from '@/ui';
import classes from './InfoModal.module.css';

interface InfoModalProps {
  isOpen: boolean;
  onClose: () => void;
}

export const InfoModal = ({ isOpen, onClose }: InfoModalProps) => {
  if (!isOpen) return null;

  return (
    <div
      className={classes.overlay}
      onClick={onClose}
      role="dialog"
      aria-modal="true"
    >
      <div className={classes.modalContainer}>
        <div className={classes.modal} onClick={(e) => e.stopPropagation()}>
          <button
            className={classes.closeButton}
            onClick={onClose}
            aria-label="Close info modal"
            type="button"
          >
            <CloseIcon width={24} height={24} />
          </button>

          <div className={classes.content}>
            <div className={classes.titleContainer}>
              <img src="/images/logo.png" className={classes.logo} />
              <h1 className={classes.title}>Rubik</h1>
            </div>

            <section className={classes.section}>
              <h2 className={classes.sectionTitle}>Features</h2>
              <ul className={classes.featureList}>
                <div className={classes.featureListItemContainer}>
                  <CubeIcon />
                  <li>Interactive 3D Rubik's Cube</li>
                </div>
                <div className={classes.featureListItemContainer}>
                  <CubeIcon />
                  <li>Color customization for each face</li>
                </div>
                <div className={classes.featureListItemContainer}>
                  <CubeIcon />
                  <li>Automatic solver algorithm</li>
                </div>
                <div className={classes.featureListItemContainer}>
                  <CubeIcon />
                  <li>Move tracking and replay</li>
                </div>
                <div className={classes.featureListItemContainer}>
                  <CubeIcon />
                  <li>Shuffle functionality</li>
                </div>
                <div className={classes.featureListItemContainer}>
                  <CubeIcon />
                  <li>Real-time validation</li>
                </div>
              </ul>
            </section>

            <section className={classes.section}>
              <h2 className={classes.sectionTitle}>Support</h2>
              <div className={classes.support}>
                <Button className={classes.supportLinkButton}>
                  <a
                    href={config.repoAddress}
                    target="_blank"
                    rel="noopener noreferrer"
                    className={classes.link}
                  >
                    <GitHubIcon width={20} height={20} />
                    GitHub Repository
                  </a>
                </Button>
              </div>
            </section>

            <div className={classes.footer}>
              <span>Made by Amit</span>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};
