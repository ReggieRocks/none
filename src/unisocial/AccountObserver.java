package unisocial;

public interface AccountObserver {
    void update(Post post);

    String getAccountId();

    String getPlatformName();
}
