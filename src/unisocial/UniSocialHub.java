package unisocial;

import java.util.Collection;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Objects;

/**
 * Concrete subject that manages connected social media accounts.
 */
public class UniSocialHub extends UniSocialSubject {
    private final String hubName;
    private final Map<String, AccountObserver> connectedAccounts = new LinkedHashMap<>();

    public UniSocialHub(String hubName) {
        this.hubName = Objects.requireNonNull(hubName, "hubName cannot be null");
    }

    @Override
    public void addSocialAccount(AccountObserver observer) {
        Objects.requireNonNull(observer, "observer cannot be null");
        connectedAccounts.put(observer.getAccountId(), observer);
        System.out.printf(
            "[UniSocial:%s] Connected %s account '%s'%n",
            hubName,
            observer.getPlatformName(),
            observer.getAccountId()
        );
    }

    @Override
    public void removeSocialAccount(String accountId) {
        AccountObserver removed = connectedAccounts.remove(accountId);
        if (removed == null) {
            System.out.printf(
                "[UniSocial:%s] Account '%s' was not connected%n",
                hubName,
                accountId
            );
            return;
        }

        System.out.printf(
            "[UniSocial:%s] Disconnected %s account '%s'%n",
            hubName,
            removed.getPlatformName(),
            removed.getAccountId()
        );
    }

    @Override
    public void broadcastPost(String content) {
        if (connectedAccounts.isEmpty()) {
            System.out.printf("[UniSocial:%s] No accounts connected. Broadcast skipped.%n", hubName);
            return;
        }

        Post post = new Post(content);
        System.out.printf(
            "[UniSocial:%s] Broadcasting post '%s' to %d account(s)%n",
            hubName,
            post.getContent(),
            connectedAccounts.size()
        );
        super.notifyAccounts(post, connectedAccounts.values());
    }

    public int getConnectedAccountCount() {
        return connectedAccounts.size();
    }
}
