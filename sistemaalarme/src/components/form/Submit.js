import styles from './Submit.module.css'

function Submit({text, onClick, style}){

    return(
        
            <div className={styles.container_btn}><button className={style} onClick={onClick} type='submit'> {text} </button></div>
        
    )
}
export default Submit