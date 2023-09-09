
import { useFirebase } from '../FirebaseContext'

const Navbar = () => {
    const { firebaseAuth, userId, setUserId } = useFirebase()

    const buttonLogoutHandler = () => {
        firebaseAuth.signOut()
            .then(() => {
                setUserId(null)
            })
            .catch((error) => {
                console.error('Error logging out:', error);
            });
    }

    return (
        <>
            <div>Irrigation Controller</div>
            {userId && <button onClick={buttonLogoutHandler}>Logout</button>}
        </>
    );
}

export default Navbar;