package unisocial;

import java.util.Collection;

/**
 * Abstract Subject in the Observer pattern.
 */
public abstract class UniSocialSubject {

    public abstract void addSocialAccount(AccountObserver observer);

    public abstract void removeSocialAccount(String accountId);

    public abstract void broadcastPost(String content);

    protected void notifyAccounts(Post post, Collection<AccountObserver> observers) {
        for (AccountObserver observer : observers) {
            observer.update(post);
        }
    }
}
