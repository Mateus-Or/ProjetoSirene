import styles from './Confirmacao.module.css'


function Confirmacao ({ isOpen, onClose, onConfirm, text }){
  if (!isOpen) return null;

  return (
    <div className={styles.modal_overlay}>
      <div className={styles.modal}>
        <h2 className={styles.titulo}>{text}</h2>
        <button className={styles.close_button} onClick={onClose}>
          Cancelar
        </button>
        <button className={styles.confirm_button} onClick={onConfirm}>
          Confirmar
        </button>
      </div>
    </div>
  );
};

export default Confirmacao;